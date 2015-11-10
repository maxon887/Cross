#include "Cross.h"
#include "Demo.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	//((LauncherWIN*)launcher)->SetTargetSize(540, 960);
	((LauncherWIN*)launcher)->SetTargetSize(512, 512);
	return new Demo(launcher);
}