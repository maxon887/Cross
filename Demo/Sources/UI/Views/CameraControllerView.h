#pragma once
#include "Cross.h"
#include "View.h"

using namespace cross;

class CameraControllerView : public View {
public:
	CameraControllerView();

	void PreUpdate() override;
	void Update(float sec) override;
	void PostUpdate() override;

	bool VisibleInMenu() override;
	bool AvailableInMenu() override;

private:
	bool nav_pressed = false;
};
