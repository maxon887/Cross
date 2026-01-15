#pragma once
#include "Game.h"
#include "Input.h"
#include "Base/String.h"

#define SCALED(x) (x * cross::os->GetScreenScale())

using namespace cross;
//
struct ImDrawData;
struct ImFont;
struct ImGuiContext;

class AutoTests;
class Demo;
class MenuBar;
class LaunchView;

extern Demo* demo;

class Demo : public Game {
public:
	static String GetCompactSize(U64 bytes);

	ImFont* small_font		= nullptr;
	ImFont* normal_font		= nullptr;
	ImFont* big_font		= nullptr;

	void Start() override;
	void Stop() override;
	void PreUpdate(float sec) override;
	void Update(float sec) override;
	void PostUpdate(float sec) override;
	void SetScreen(Screen* screen) override;

	void ToMain();
	bool LoadScene(const String& filename);
	LaunchView* GetLaunchView();
	MenuBar* GetMenuBar();
	AutoTests* GetAutoTests();

private:
	static const int MaxInputActions = 5;
	static const char* GetClipboardString(ImGuiContext* context);
	static void SetClipboardString(ImGuiContext* context, const char* data);

	AutoTests* auto_tests		= nullptr;
	MenuBar* menu				= nullptr;
	LaunchView* launch_view 	= nullptr;
	Shader* ui_shader			= nullptr;
	Texture* font_texture		= nullptr;
	String clipboard			= "";
	String imgui_filename		= "imgui.ini";
	U32 vertex_buffer			= 0;
	U32 index_buffer			= 0;

	Vector2D action_pos			= Vector2D(0.f, 0.f);
	Array<bool> actions_down 	= Array<bool>(MaxInputActions, false);
	Array<bool> actions_up		= Array<bool>(MaxInputActions, false);
	float mouse_wheel			= 0.0f;

    bool CreateUIShaders();
	bool CreateFontsTexture();
	void RenderUI(ImDrawData*);

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	void KeyPressed(Key key);
	void KeyReleased(Key key);
	void CharEnter(char c);

	void WheelRoll(float delta);
};
