#pragma once
#include "Cross.h"
#include "View.h"

using namespace cross;

class LaunchView : public View {
public:
	LaunchView();

	void PreUpdate() override;
	void Update(float sec) override;
	void PostUpdate() override;
};
