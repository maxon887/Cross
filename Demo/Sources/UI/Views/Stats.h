#pragma once
#include "Cross.h"
#include "UI/Views/View.h"

using namespace cross;

class Stats : public View {
public:
	Stats() : View("Stats") { }

	void PreUpdate() override;
	void Update(float sec) override;
};
