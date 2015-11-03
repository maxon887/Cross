#include "Cross.h"
#include "LauncherWIN.h"
#include "SliceThemOut.h"

Game* CrossMain(Launcher* launcher){
	LauncherWIN* winLan = (LauncherWIN*)launcher;
	winLan->SetTargetSize(960, 540);
	return new SliceThemOut(launcher);
}