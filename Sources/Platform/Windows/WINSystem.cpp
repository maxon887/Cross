/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "WINSystem.h"
#include "File.h"
#include "Config.h"

#include "shlwapi.h"

#define DATA_PATH "Data/"

#pragma comment(lib, "Shlwapi.lib")

using namespace cross;

void IntSleep(int milis) {
	Sleep(milis);
}

WINSystem::WINSystem(HWND wnd) :
	wnd(wnd)
{
	LogIt("LauncherWIN::LauncherWIN(HWND wnd)");
	char* releaseAsset = "Assets/";
	char* debugAsset = "../../../Assets/";
	char* debugAssetAlt = "../../Assets/";
	char* editorAsset = "../Demo/Assets/";
	if(IsDirectoryExists(releaseAsset)) {
		assets_path = releaseAsset;
	} else if(IsDirectoryExists(debugAsset)) {
		assets_path = debugAsset;
	} else if(IsDirectoryExists(debugAssetAlt)) {
		assets_path = debugAssetAlt;
	} else if(IsDirectoryExists(editorAsset)) {
		assets_path = editorAsset;
	} else {
		CROSS_ASSERT(false, "Can't find Assets folder");
	}
}

void WINSystem::Log(const char* msg) {
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
}

String WINSystem::AssetsPath() {
	return assets_path;
}

String WINSystem::DataPath() {
	return DATA_PATH;
}

U64 WINSystem::GetTime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER crt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&crt);
	return (crt.QuadPart * 1000000) / freq.QuadPart;
}

float WINSystem::GetScreenDPI() {
	SetProcessDPIAware(); //true
	HDC screen = GetDC(nullptr);
	double hPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSX);
	double vPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(nullptr, screen);
	return (float)(hPixelsPerInch + vPixelsPerInch) * 0.5f;
}

String WINSystem::GetClipboard() {
	CROSS_RETURN(OpenClipboard(nullptr), "", "Can not open clipboard data");
	HANDLE hData = GetClipboardData(CF_TEXT);
	CROSS_RETURN(hData, "", "Clipboard data == nullptr");
	char* text = static_cast<char*>(GlobalLock(hData));
	CROSS_RETURN(text, "", "Text pointer == nullptr");
	clipboard = text;
	GlobalUnlock(hData);
	CloseClipboard();
	return clipboard;
}

bool WINSystem::Alert(const String& msg) {
	if(wnd) {
		auto msgBoxResult = MessageBoxA(wnd, msg.ToCStr(), "Something goes wrong", MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
		switch(msgBoxResult) {
		case IDIGNORE:
			return true;
		case IDABORT:
			*((unsigned int*)0) = 0xDEAD;
		default:
			return false;
		}
	} else {
		LogIt("HWND == nullptr");
		System::Messagebox("Something goes wrong", msg);
	}
	return false;
}

void WINSystem::Messagebox(const String& title, const String& msg) {
	if(wnd) {
		MessageBoxA(wnd, msg.ToCStr(), title.ToCStr(), MB_OK | MB_ICONEXCLAMATION);
	} else {
		LogIt("HWND == nullptr");
		System::Messagebox(title, msg);
	}
}

bool WINSystem::IsDirectoryExists(const String& filepath) {
	DWORD dwAttrib = GetFileAttributes(filepath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void WINSystem::CreateDirectory(const String& dirpath) {
	CROSS_ASSERT(CreateDirectoryA(dirpath, nullptr), "Can not create directory");
}

Array<String> WINSystem::GetSubDirectories(const String& filepath) {
	Array<String> result;

	HANDLE file = nullptr;
	WIN32_FIND_DATA data;
	file = FindFirstFile(filepath + "*", &data);
	if(file != INVALID_HANDLE_VALUE) {
		do {
			String filename = data.cFileName;
			if((data.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY
				&& filename != "." && filename != "..") {
				result.Add(data.cFileName);
			}
		} while(FindNextFile(file, &data));
		FindClose(file);
	}

	return result;
}

Array<String> WINSystem::GetFilesInDirectory(const String& directory) {
	Array<String> result;

	HANDLE file = nullptr;
	WIN32_FIND_DATA data;
	file = FindFirstFile(directory + "*", &data);
	if(file != INVALID_HANDLE_VALUE) {
		do {
			String filename = data.cFileName;
			if((data.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY
				&& filename != "." && filename != "..") {
				result.Add(data.cFileName);
			}
		} while(FindNextFile(file, &data));
		FindClose(file);
	}

	return result;
}

void WINSystem::Sleep(float milis) {
	IntSleep((int)(milis + .5));
}

bool WINSystem::IsMobile() {
	return config->GetBool("EMULATE_MOBILE", false);
}

void WINSystem::OpenFileExternal(const String& filename) {
	char szFileName[MAX_PATH + 1];
	GetCurrentDirectoryA(MAX_PATH + 1, szFileName);
	String fullpath = String(szFileName) + "\\" + AssetsPath() + filename;
	S32 result = (S32)ShellExecuteA(NULL, NULL, fullpath.ToCStr(), NULL, NULL, SW_SHOW);
	if(result < 32) {
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		CROSS_ASSERT(false, String("Can not open file with external editor\nerror:") + messageBuffer);
	}
}

String WINSystem::OpenFileDialog(const String& extension /* *.* */, bool saveDialog /* = false */) {
	char szFile[512];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	char fileFilter[512];
	memset(fileFilter, 0, 512);
	strcpy(fileFilter, extension.ToCStr());
	memcpy(fileFilter + strlen(fileFilter) + 1, extension.ToCStr(), extension.Length() + 1);
	ofn.lpstrFilter = fileFilter;
	//ofn.lpstrFilter = "File Type\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	String initPath = system->AssetsPath();
	initPath.Replace("/", "\\");
	ofn.lpstrInitialDir = initPath;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if(saveDialog) {
		GetSaveFileName(&ofn);
		return szFile;
	} else {
		GetOpenFileName(&ofn);
	}
	String filepath = szFile;
	if(filepath == "") {
		return filepath;
	}

	char currentDir[512];
	GetCurrentDirectory(511, currentDir);

	char pathToAssets[512];
	String winAssetsPath = AssetsPath();
 	winAssetsPath.Replace("/", "\\");
	PathCombine(pathToAssets, currentDir, winAssetsPath);

	char relativePath[512];
	PathRelativePathTo(relativePath, pathToAssets, FILE_ATTRIBUTE_DIRECTORY, filepath, FILE_ATTRIBUTE_NORMAL);

	String result = relativePath;
	result.Replace("\\", "/");

	return result;
}

void WINSystem::SetAssetPath(const String& path) {
	assets_path = path;
}

void WINSystem::FullScreen(bool yes) {
	if(yes){
		HDC dc = GetDC(wnd);
		int fullscreenWidth = GetDeviceCaps(dc, HORZRES);
		int fullscreenHeight = GetDeviceCaps(dc, VERTRES);
		int colourBits = GetDeviceCaps(dc, BITSPIXEL);
		int refreshRate = GetDeviceCaps(dc, VREFRESH);

		EnterFullscreen(wnd, fullscreenHeight, fullscreenWidth, colourBits, refreshRate);
	}else{
		ExitFullscreen(wnd, config->GetInt("WIN_POS_X", 0), config->GetInt("WIN_POS_Y", 0), config->GetInt("WIN_WIDTH", 500), config->GetInt("WIN_HEIGHT", 500), 0, 0);
	}
}

void WINSystem::ResizeWindow(int posX, int posY, int width, int height) {
	CROSS_FAIL(wnd && !fullscreen, "Zero WND. Can not resize native window");
	window_pos_x = posX;
	window_pos_y = posY;
	SetWindowSize(width, height);
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(wnd, &rcClient);
	GetWindowRect(wnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(wnd, posX, posY, width + ptDiff.x, height + ptDiff.y, TRUE);
}

void WINSystem::SetWND(HWND wnd) {
	this->wnd = wnd;
}

HWND WINSystem::GetHWND() {
	return wnd;
}

void WINSystem::SetWindowPosition(int x, int y) {
	window_pos_x = x;
	window_pos_y = y;
}

void WINSystem::KeyReleasedHandle(Key key) {
	switch(key)	{
	case Key::F1:	//16:9
		ResizeWindow(window_pos_x, window_pos_y, 960, 540);
		//ResizeWindow(window_pos_x, window_pos_y, 1600, 900);
		break;
	case Key::F2:	//9:16
		ResizeWindow(window_pos_x, window_pos_y, 540, 960);
		//ResizeWindow(window_pos_x, window_pos_y, 900, 1600);
		break;
	case Key::F3:	//4:3
		//ResizeWindow(window_pos_x, window_pos_y, 800, 600);
		ResizeWindow(window_pos_x, window_pos_y, 1280, 960);
		break;
	case Key::F4:	//3:4
		//ResizeWindow(window_pos_x, window_pos_y, 600, 800);
		ResizeWindow(window_pos_x, window_pos_y, 960, 1280);
		break;
	case Key::F5:	//8:5
		ResizeWindow(window_pos_x, window_pos_y, 1280, 800);
		break;
	case Key::F6:	//5:8
		ResizeWindow(window_pos_x, window_pos_y, 800, 1280);
		break;
	case Key::F8:
		fullscreen = !fullscreen;
		FullScreen(fullscreen);
		break;
	default:
		break;
	}
}

bool WINSystem::EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate) {
	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;

	EnumDisplaySettings(nullptr, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = fullscreenWidth;
	fullscreenSettings.dmPelsHeight = fullscreenHeight;
	fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;
	fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hwnd, SW_MAXIMIZE);

	return isChangeSuccessful;
}

bool WINSystem::ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY) {
	bool isChangeSuccessful;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LEFT);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	isChangeSuccessful = ChangeDisplaySettings(nullptr, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
	SetWindowPos(hwnd, HWND_NOTOPMOST, windowX, windowY, windowedWidth + windowedPaddingX, windowedHeight + windowedPaddingY, SWP_SHOWWINDOW);
	ShowWindow(hwnd, SW_RESTORE);

	return isChangeSuccessful;
}