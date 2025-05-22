#pragma once

#include "System.h"

namespace cross {

class LinuxSystem : public System {
public:
	String AssetsPath() override;
	String DataPath() override;
	U64 GetTime() override;
};

}