#include "Cross.h"
#include "SliceThemOut.h"

Game* CrossMain(Launcher* launcher){
	return new SliceThemOut(launcher);
}