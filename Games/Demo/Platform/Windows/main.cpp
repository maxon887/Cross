#include "Cross.h"
#include "Demo.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	//((LauncherWIN*)launcher)->SetTargetSize(540, 800);
	((LauncherWIN*)launcher)->SetTargetSize(500, 500);
	Game* demo = new Demo(launcher);
	return demo;
}