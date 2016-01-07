#include "Cross3D.h"
#include "MainScreen.h"
#include "HelloTriangleScreen.h"
#include "PrimitivesScreen.h"
#include "AdvancedScreen.h"
#include "Screen2D.h"

Screen* Cross3D::GetStartScreen(){
	//return new HelloTriangleScreen();
	//return new PrimitivesScreen();
	//return new AdvancedScreen();
	return new Screen2D();
}