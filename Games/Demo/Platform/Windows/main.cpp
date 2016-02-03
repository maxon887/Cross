#include "Cross.h"
#include "Demo.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	//((LauncherWIN*)launcher)->SetTargetSize(540, 800);
	//((LauncherWIN*)launcher)->SetTargetSize(540, 800);
	Game* demo = new Demo(launcher);
	return demo;
}