#include "Cross.h"
#include "JimTheSnake.h"
#include "LauncherWIN.h"

Game* CrossMain(Launcher* launcher){
	((LauncherWIN*)launcher)->SetTargetSize(540, 960);
	return new JimTheSnake(launcher);
}