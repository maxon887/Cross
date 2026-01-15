#pragma once
#include "Screen.h"

using namespace cross;

class ImGuiScreen : public Screen {
public:
	void Update(float sec) override;

private:
	bool show_test_window = true;
	bool show_another_window = false;
};