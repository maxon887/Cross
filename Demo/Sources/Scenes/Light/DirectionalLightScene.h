#pragma once
#include "Scenes/DemoScene.h"

class DirectionalLightScene : public DemoScene {
public:
	void Start() override;
	void Stop() override;

private:
	Shader* shader				= nullptr;
	Material* material			= nullptr;
	Entity* cube				= nullptr;
};