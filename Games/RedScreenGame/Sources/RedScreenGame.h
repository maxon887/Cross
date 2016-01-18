#pragma once

#include "Game.h"

using namespace cross;

class RedScreenGame : public Game {
public:
	RedScreenGame(Launcher* launcher);
	void Start();
	Screen* GetStartScreen();
};

