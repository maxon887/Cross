#include "ImGuiScreen.h"

#include "ThirdParty/ImGui/imgui.h"

void ImGuiScreen::Update(float sec) {
	Screen::Update(sec);

	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	if(ImGui::Button("Test Window")) show_test_window ^= 1;
	if(ImGui::Button("Another Window")) show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if(show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowDemoWindow();
	}
}