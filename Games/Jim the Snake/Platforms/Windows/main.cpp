#include "Cross.h"
#include "JimTheSnake.h"

Game* CrossMain(Launcher* launcher){
	return new JimTheSnake(launcher);
}