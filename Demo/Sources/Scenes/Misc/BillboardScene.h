#pragma once

#include "Scenes/DemoScene.h"

class BillboardScene : public DemoScene{
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Material* cubeMaterial = nullptr;
	Material* billboardMaterial = nullptr;
	Entity* billboard = nullptr;
};
