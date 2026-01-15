#include "MenuBar.h"
#include "System.h"
#include "Demo.h"
#include "Transform.h"
#include "UI/Views/CameraControllerView.h"
#include "UI/Views/SceneView.h"
#include "UI/Views/FilesView.h"
#include "UI/Views/ComponentsView.h"
#include "UI/Views/Log.h"
#include "UI/Views/Stats.h"
#include "UI/Views/About.h"
#include "UI/Views/LaunchView.h"
#include "UI/Views/PropertiesView.h"
#include "Scenes/DemoScene.h"
#include "File.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

MenuBar::MenuBar() {
	CameraControllerView* cameraController = CREATE CameraControllerView();
	files_view = CREATE FilesView();
	SceneView* sceneView = CREATE SceneView();
	ComponentsView* componentsView = CREATE ComponentsView(sceneView, files_view);
	PropertiesView* propertiesView = CREATE PropertiesView(files_view);
	views.Add(files_view);
	views.Add(sceneView);
	views.Add(componentsView);
	views.Add(propertiesView);
	views.Add(cameraController);
	for(View* v : views) {
		if(v->IsVisible()) {
			v->Shown();
		}
	}

	log = CREATE Log();
	stats = CREATE Stats();
	about = CREATE About();

	input->KeyPressed.Connect(this, &MenuBar::KeyPressed);
}

MenuBar::~MenuBar() {
	for(View* v : views) {
		delete v;
	}
	views.Clear();

	delete about;
	delete stats;
	delete log;
}

void MenuBar::Update(float sec) {
	UpdateDocking();
	for(View* v : views) {
		v->Run(sec);
	}
	if(show_style_editor) {
		ImGui::Begin("Style Editor", &show_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	log->Run(sec);
	stats->Run(sec);
	about->Run(sec);
}

void MenuBar::ShowMenu() {
	if(os->IsMobile()) {
		ImGui::PushFont(demo->big_font);
	}

	bool haveScene = game->GetCurrentScene() != nullptr;
	//shortcuts implementation
	if((input->IsPressed(Key::COMMAND) || input->IsPressed(Key::CONTROL)) && input->IsPressed(Key::S) && haveScene ) {
		SaveScene();
	}
	if((input->IsPressed(Key::COMMAND) || input->IsPressed(Key::CONTROL)) && input->IsPressed(Key::X) && haveScene ) {
		demo->ToMain();
	}
	
	if(ImGui::BeginMainMenuBar()) {
		if(os->IsMobile()) {
			ImGui::SetCursorPosX(SCALED(35.f));
		}
		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("New Scene")) {
				Scene* scene = CREATE DemoScene();
				game->SetScreen(scene);
			}

			if(ImGui::MenuItem("Open Scene", NULL, false, !os->IsMobile())) {
				String sceneFile = os->OpenFileDialog("*.scn");
				if(sceneFile != "") {
					Scene* scene = CREATE DemoScene();
					if(!scene->Load(sceneFile)) {
						delete scene;
						CROSS_ASSERT(false, "Can not load scene file, sorry");
					} else {
						game->SetScreen(scene);
					}
				}
			}
#ifdef MACOS
			static const char* saveShortcut = "Cmd+S";
#else
			static const char* saveShortcut = "Ctrl+S";
#endif
			if(ImGui::MenuItem("Save Scene", os->IsMobile() ? "" : saveShortcut, false, haveScene && !os->IsMobile())) {
				SaveScene();
			}
#ifdef MACOS
			static const char* backShortcut = "Cmd+X";
#else
			static const char* backShortcut = "Ctrl+X";
#endif
			if(ImGui::MenuItem("Back to Main", os->IsMobile() ? "" : backShortcut, false, !demo->GetLaunchView()->IsVisible())) {
				demo->ToMain();
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("View")) {
			for(View* v : views) {
				if(v->VisibleInMenu()) {
					bool selected = v->IsVisible();
					bool available = v->AvailableInMenu();
					if(ImGui::MenuItem(v->GetName(), "", &selected, available)) {
						if(selected) {
							v->Show();
						} else {
							v->Hide();
						}
					}
				}
			}
			ImGui::EndMenu();
		}

		ImVec2 helpSize = ImGui::CalcTextSize("Help");
		ImGui::SameLine(ImGui::GetWindowWidth() - helpSize.x * 2.f);
		if(ImGui::BeginMenu("Help")) {
			if(ImGui::MenuItem("Log")) {
				log->Show();
			}
			if(ImGui::MenuItem("Stats")) {
				stats->Show();
			}
			if(ImGui::MenuItem("Style Editor")) {
				show_style_editor = true;
			}
			if(ImGui::MenuItem("About")) {
				about->Show();
			}
			ImGui::EndMenu();
		}
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 size = ImGui::GetItemRectSize();
		menu_height = size.y +style.FramePadding.y * 2.f - style.ItemSpacing.y * 2.f;// +style.ItemSpacing.y;
		ImGui::EndMainMenuBar();
	}

	if(os->IsMobile()) {
		ImGui::PopFont();
	}
}

void MenuBar::CloseAllViews() {
	for(View* v : views) {
		v->Hide();
	}
}

float MenuBar::GetHeight() const {
	return menu_height;
}

void MenuBar::UpdateDocking() {
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		host_window_flags |= ImGuiWindowFlags_NoBackground;

	char label[32];
	ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(label, NULL, host_window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceID = ImGui::GetID("DockSpace");

	ImGuiDockNode* DockNode = ImGui::DockBuilderGetNode(dockspaceID);

	if(!DockNode)	//default docking configuration
	{
		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);


		ImGuiID dockspace_id_copy = dockspaceID;
		ImGuiID left = ImGui::DockBuilderSplitNode(dockspace_id_copy, ImGuiDir_Left, 0.3f, nullptr, &dockspace_id_copy);
		ImGuiID right = ImGui::DockBuilderSplitNode(dockspace_id_copy, ImGuiDir_Right, 0.45f, nullptr, &dockspace_id_copy);
		for(View* v : views) {
			switch(v->GetDefaultDockPosition())
			{
			case View::DockPosition::LEFT:
				ImGui::DockBuilderDockWindow(v->GetName(), left);
				break;
			case View::DockPosition::RIGHT:
				ImGui::DockBuilderDockWindow(v->GetName(), right);
				break;
			default:
				break;
			}
		}
		ImGui::DockBuilderFinish(dockspaceID);
	}

	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspace_flags, NULL);
	ImGui::End();
}

void MenuBar::SaveScene() {
	String filename = os->OpenFileDialog("*.scn", true);
	if(filename != "") {
		String extension = File::ExtensionFromFile(filename);
		if(extension.IsEmpty()) {
			filename += ".scn";
		}
		game->GetCurrentScene()->Save(filename);
	}
}

void MenuBar::KeyPressed(cross::Key key) {
	if((key == Key::ENTER && input->IsPressed(Key::ALT)) || (key == Key::ALT && input->IsPressed(Key::ENTER))) {
		fullscreen = !fullscreen;
		os->Fullscreen(fullscreen);
	}
}