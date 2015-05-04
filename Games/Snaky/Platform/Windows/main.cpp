#include "Cross.h"
#include "SnakyGame.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR args, int winShow){
	LauncherWIN* launcher = new LauncherWIN();
	SnakyGame* game = new SnakyGame(launcher);
	return CrossMain(game, instance, winShow);
}