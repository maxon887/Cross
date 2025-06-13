#pragma once

#include "System.h"

namespace cross {

class LinuxSystem : public System {
public:
	LinuxSystem();

	String AssetsPath() override;
	String DataPath() override;
	U64 GetTime() override;

	bool Alert(const String& msg) override;
	void Sleep(float milis) override;

private:
	String assets_path = "";
};

}