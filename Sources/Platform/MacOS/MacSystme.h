#pragma once
#include "System.h"

namespace cross {

class MacSystem : public System {
public:
    void Log(const char* msg) override;
    String AssetsPath() override;
    String DataPath() override;
    U64 GetTime() override;
};

}
