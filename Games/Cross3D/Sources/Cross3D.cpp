#include "Cross3D.h"
#include "MainScreen.h"
#include "HelloTriangleScreen.h"
#include "PrimitivesScreen.h"
#include "AdvancedScreen.h"

Screen* Cross3D::GetStartScreen(){
	//return new HelloTriangleScreen();
	//return new PrimitivesScreen();
	return new AdvancedScreen();
}