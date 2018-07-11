#pragma once
#include "Cross.h"

using namespace cross;

class TransformGizmo {
public:
	TransformGizmo();
	~TransformGizmo();

	void Update(float sec);

private:
	Shader* shader		= nullptr;
	Material* red_mat	= nullptr;
	Material* green_mat = nullptr;
	Material* blue_mat	= nullptr;
	Model* arrow		= nullptr;
	Entity* entity		= nullptr;
};