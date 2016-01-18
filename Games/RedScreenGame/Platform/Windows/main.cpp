#include "Cross.h"
#include "RedScreenGame.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	//((LauncherWIN*)launcher)->SetTargetSize(540, 800);
	((LauncherWIN*)launcher)->SetTargetSize(500, 500);
	Game* red_Screen_Game = new RedScreenGame(launcher);
	return red_Screen_Game;
}