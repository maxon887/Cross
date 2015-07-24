#include "Cross.h"
#include "Demo.h"

Game* CrossMain(Launcher* launcher){
	return new Demo(launcher);
}