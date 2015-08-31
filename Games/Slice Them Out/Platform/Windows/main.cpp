#include "Cross.h"
#include "LauncherWIN.h"
#include "SliceThemOut.h"

Game* CrossMain(Launcher* launcher){
	((LauncherWIN*)launcher)->LandscapeMode(true);
	return new SliceThemOut(launcher);
}