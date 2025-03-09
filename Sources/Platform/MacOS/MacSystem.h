#pragma once
#include "System.h"

namespace cross {

class MacSystem : public System {
public:
    MacSystem(const String& executable);
    
    void Log(const char* msg) override;
    String AssetsPath() override;
    String DataPath() override;
    U64 GetTime() override;
	float GetScreenDPI() override;
	bool Alert(const String& msg) override;
	void Messagebox(const String& title, const String& msg) override;
	void Sleep(float milis) override;
	String OpenFileDialog(const String& extension = "*.*", bool saveDialog = false) override;

    void SetScreenDPI(float dpi);
	void SetAssetPath(const String& path);
	
	void SetAppIcon();

private:
    float dpi = 96.f;

	String assets_path = "";
	String data_path = "";
};

}
