#include "About.h"

#include "ThirdParty/ImGui/imgui.h"

void About::Update(float sec) {
	ImGui::Text("Cross++, v%s", cross::version);
	ImGui::Separator();
	ImGui::Text("By Maksim Lukyanov");
}