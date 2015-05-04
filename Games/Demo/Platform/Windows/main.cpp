#include "Cross.h"
#include "Demo.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR args, int winShow){
	LauncherWIN* launcher = new LauncherWIN();
	Demo* game = new Demo(launcher);
	return CrossMain(game, instance, winShow);
}