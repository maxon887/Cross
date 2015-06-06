#include "LauncherWIN.h"
#include "Cross.h"
#include "JimTheSnake.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR args, int winShow){
	LauncherWIN* launcher = new LauncherWIN();
	JimTheSnake* game = new JimTheSnake(launcher);
	return CrossMain(game, instance, winShow);
}