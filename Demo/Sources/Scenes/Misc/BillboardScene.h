#pragma once

#include "Scenes/DemoScene.h"

class BillboardScene : public DemoScene{
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Material* material = nullptr;
};
