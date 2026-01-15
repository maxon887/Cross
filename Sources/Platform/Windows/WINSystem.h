#pragma once
#include "System.h"
#include "Input.h"

#include <Windows.h>
#undef CreateDirectory

namespace cross{

class WINSystem : public System {
public:
	WINSystem(HWND wnd = 0);

	void Log(const char* msg) override;
	String AssetsPath() override;
	String DataPath() override;
	U64 GetTime() override;
	float GetScreenDPI() override;
	String GetClipboard() override;
	void SetClipboard(const String& data) override;

	bool Alert(const String& msg) override;
	void Messagebox(const String& title, const String& msg) override;
	void Sleep(float milis) override;
	bool IsMobile() override;
	void OpenFileExternal(const String& filename) override;
	String OpenFileDialog(const String& extension = "*.*", bool saveDialog = false) override;
	void Fullscreen(bool enable) override;

	void SetAssetPath(const String& path);
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

	String GetLastErrorString(DWORD err);

	bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
	bool ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);
};

}