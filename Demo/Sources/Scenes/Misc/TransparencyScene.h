#pragma once
#include "Scenes/DemoScene.h"
#include "Utils/Skybox.h"

class TransparencyScene : public DemoScene{
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Shader* road_shader		= nullptr;
	Shader* grass_shader	= nullptr;
	Shader* sphere_shader	= nullptr;
	Material* road_mat		= nullptr;
	Material* grass_mat		= nullptr;
	Material* sphere_mat	= nullptr;

	Entity* light			= nullptr;
};
