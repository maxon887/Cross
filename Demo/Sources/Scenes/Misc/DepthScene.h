#pragma once
#include "Scenes/DemoScene.h"

class DepthScene : public DemoScene{
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Shader* shader		= nullptr;
	Material* car_mat	= nullptr;
	Material* road_mat	= nullptr;
};
