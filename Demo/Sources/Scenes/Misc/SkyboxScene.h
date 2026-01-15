#pragma once
#include "Scenes/DemoScene.h"
#include "Utils/Skybox.h"

class SkyboxScene : public DemoScene {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Skybox* skybox		= nullptr;
	Material* material	= nullptr;
};