#include <iostream>
#include <Windows.h>

#include "Cross.h"
#include "Cross3D.h"

Game* CrossMain(Launcher* launcher){
	return new Cross3D(launcher);
}
