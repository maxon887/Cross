#pragma once

#include "Cross.h"

#include <cfloat>

using namespace cross;

class AutoTests {
public:
	void StartTests();
	void Update(float sec);

private:
	enum Test {
		TRIANGLE_SCENE,
		CUBE_SCENE,
		TEXTURED_CUBE,
		MATERIAL,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		MULTI_LIGHT,
		NAKED_MODEL,
		DIFFUSE_MODEL,
		SPECULAR_MODEL,
		ROUGHNESS_MODEL,
		NORMAL_MODEL,
		DEPTH_TEST,
		TRANSPARENCY,
		SKYBOX,
		NONE,
	};

	Test current_test = NONE;
	float next_test_time = FLT_MAX;


	void OnScreenChanged(Screen* screen);
};