#pragma once
#include "Cross.h"
#include "View.h"

#include "ThirdParty/ImGui/imgui.h"

using namespace cross;

class Log : public View {
public:
	Log() : View("Log") { }

	void PreUpdate() override;
	void Update(float sec) override;

private:
	ImGuiTextFilter     filter;
	U64					log_size = 0;
	bool				scroll_to_bottom = true;

	void Clear();
};
