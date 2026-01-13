#pragma once

#include "Cross.h"

#include <cfloat>

using namespace cross;

class AutoTests {
public:
	void StartTests();
	void Update(float sec);

private:
	bool TestsActive = false;
	float TriangleScreenLoaded = FLT_MAX;

	void OnScreenChanged(Screen* screen);
};