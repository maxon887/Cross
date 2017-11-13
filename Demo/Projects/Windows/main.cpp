#include "Cross.h"
#include "Demo.h"
#include "Platform/Windows/WINSystem.h"

Game* CrossMain() {
	Game* demo = new Demo();
	return demo;
}