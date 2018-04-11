#pragma once
#include "System.h"

namespace cross {

class MacSystem : public System {
public:
    void Log(const char* msg) override;
    String AssetsPath() override;
    String DataPath() override;
    U64 GetTime() override;
    float GetScreenDPI() override;

    void SetScreenDPI(float dpi);
	void SetAssetPath(const String& path);

private:
    float dpi = 96.f;

	String assets_path = "../../../Assets/";
};

}
