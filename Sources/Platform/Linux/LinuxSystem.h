#pragma once

#include "System.h"

namespace cross {

class LinuxSystem : public System {
public:
	String AssetsPath() override;
	String DataPath() override;
	U64 GetTime() override;

	bool Alert(const String& msg) override;
	void Sleep(float milis) override;
};

}