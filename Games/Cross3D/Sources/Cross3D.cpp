#include "Cross3D.h"
#include "MainScreen.h"
#include "HelloTriangleScreen.h"

Screen* Cross3D::GetStartScreen(){
	return new HelloTriangleScreen();
}