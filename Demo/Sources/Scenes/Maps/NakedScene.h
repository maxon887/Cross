#pragma once
#include "Scenes/DemoScene.h"

class NakedScene : public DemoScene {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Shader* shader		= nullptr;
	Material* material	= nullptr;
	Entity* light		= nullptr;
};