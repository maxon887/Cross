#pragma once
#include "Cross.h"
#include "Scenes/DemoScene.h"

class MultiLightScene : public DemoScene {
public:
	void Start() override;
	void Stop() override;

protected:
	Shader* shader				= nullptr;
	Material* material			= nullptr;
	Entity* cube				= nullptr;
};
