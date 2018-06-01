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
#pragma once
#include "System.h"
#include "Input.h"

#include <Windows.h>

namespace cross{

class WINSystem : public System {
public:
	WINSystem(HWND wnd = 0);
	~WINSystem() { }

	void Log(const char* msg) override;
	String AssetsPath() override;
	String DataPath() override;
	U64 GetTime() override;
	float GetScreenDPI() override;
	String GetClipboard() override;

	void Messagebox(const String& title, const String& msg) override;
	bool IsDirectoryExists(const String& filepath) override;
	Array<String> GetSubDirectories(const String& filepath) override;
	Array<String> GetFilesInDirectory(const String& directory) override;
	void Sleep(float milis) override;
	bool IsMobile() override;
	void OpenFileExternal(const String& filename) override;

	void SetAssetPath(const String& path);
	void FullScreen(bool yes);
	void ResizeWindow(int posX, int posY, int width, int height);

public:
	void SetWND(HWND wnd);
	HWND GetHWND();
	void SetWindowPosition(int x, int y);

	void KeyReleasedHandle(Key key);
	
private:
	HWND wnd			= 0;
	String assets_path	= "";
	String clipboard	= "";
	int window_pos_x	= 0;
	int window_pos_y	= 0;
	bool fullscreen		= false;

	bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
	bool ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);
};

}