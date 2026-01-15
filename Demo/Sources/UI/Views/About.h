#pragma once
#include "Cross.h"
#include "UI/Views/View.h"

using namespace cross;

class About : public View {
public:
	About() : View("About") { }
	
	void Update(float sec) override;
};
