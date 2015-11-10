#include "Cross.h"
#include "Demo.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	((LauncherWIN*)launcher)->SetTargetSize(540, 800);
	return new Demo(launcher);
}