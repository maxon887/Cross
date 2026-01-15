#include "View.h"
#include "System.h"
#include "Config.h"

#include "ThirdParty/ImGui/imgui.h"

View::View(const String& name, bool defaultVisibility)
{
	landscape_name = name + "##Landscape";
	portrait_name = name + "##Portrait";
	String windowID = String::Format("VIEW_#_VISIBLE", GetName().Hash());
	if(os->IsMobile()) {
		defaultVisibility = false;
	}
	visible = config->GetBool(windowID, defaultVisibility);
}

View::~View() {
	String windowID = String::Format("VIEW_#_VISIBLE", GetName().Hash());
	config->SetBool(windowID, visible);
}

void View::Show() {
	visible = true;
	Shown();
}

void View::Hide() {
	visible = false;
	Hidden();
}

bool View::IsVisible() const {
	return visible;
}

const String& View::GetName() const {
	return landscape_name;
}

void View::Run(float sec) {
	if(visible) {
		PreUpdate();
		switch(os->GetDeviceOrientation()) {
		case System::Orientation::LANDSCAPE:
			ImGui::Begin(landscape_name, &visible, (ImGuiWindowFlags)flags);
			break;
		case System::Orientation::PORTRAIT:
			ImGui::Begin(portrait_name, &visible, (ImGuiWindowFlags)flags);
			break;
		default:
			CROSS_FAIL(false, "Unknown device orientation");
			break;
		}

		Update(sec);
		ImGui::End();
		PostUpdate();
	}
}

void View::SetFlags(U32 f) {
	this->flags = f;
}
