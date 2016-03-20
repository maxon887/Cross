#include "Cross.h"
#include "Demo.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	Game* demo = new Demo(launcher);
	return demo;
}