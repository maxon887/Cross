#include "Game.h"

using namespace cross;

class Cross3D : public Game{
public:
	Cross3D(Launcher* launcher):Game(launcher, 512, 512){}
	Screen* GetStartScreen();
};
