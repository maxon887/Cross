#pragma once
#include "Cross.h"
#include "Scenes/DemoScene.h"

class SpotLightScene : public DemoScene {
public:
	void Start() override;
	void Stop() override;

private:
	Shader* shader				= nullptr;
	Material* material			= nullptr;
	Entity* cube				= nullptr;
};