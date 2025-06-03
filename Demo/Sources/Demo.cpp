/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "Demo.h"
#include "Graphics.h"
#include "Screen.h"
#include "System.h"
#include "Config.h"
#include "Factory.h"
#include "Shaders/Shader.h"
#include "UI/MenuBar.h"
#include "UI/Views/LaunchView.h"
#include "Tests/Tester.h"
#include "AnimatedCameraController.h"

#include "ThirdParty/ImGui/imgui.h"

#define DEFAULT_FONT_SIZE 13.f

using namespace cross;

Demo* demo = nullptr;

ImGuiKey FromCrossKeyToImgui(Key key) {
	switch(key)	{
	case cross::Key::BACKSPACE:
		return ImGuiKey_Backspace;
	case cross::Key::TAB:
		return ImGuiKey_Tab;
	case cross::Key::COMMAND:
		return ImGuiMod_Super;
	case cross::Key::OPTION:
		return ImGuiMod_Alt;
	case cross::Key::CLEAR:
		return ImGuiKey_NamedKey_BEGIN;//does not know this key
	case cross::Key::ENTER:
		return ImGuiKey_Enter;
	case cross::Key::SHIFT:
		return ImGuiMod_Shift;
	case cross::Key::CONTROL:
		return ImGuiMod_Ctrl;
	case cross::Key::ALT:
		return ImGuiMod_Alt;
	case cross::Key::PAUSE:
		return ImGuiKey_Pause;
	case cross::Key::CAPSLOCK:
		return ImGuiKey_CapsLock;
	case cross::Key::ESCAPE:
		return ImGuiKey_Escape;
	case cross::Key::SPACE:
		return ImGuiKey_Space;
	case cross::Key::PAGE_UP:
		return ImGuiKey_PageUp;
	case cross::Key::PAGE_DOWN:
		return ImGuiKey_PageDown;
	case cross::Key::END:
		return ImGuiKey_End;
	case cross::Key::HOME:
		return ImGuiKey_Home;
	case cross::Key::LEFT:
		return ImGuiKey_LeftArrow;
	case cross::Key::UP:
		return ImGuiKey_UpArrow;
	case cross::Key::RIGHT:
		return ImGuiKey_RightArrow;
	case cross::Key::DOWN:
		return ImGuiKey_DownArrow;
	case cross::Key::INSERT:
		return ImGuiKey_Insert;
	case cross::Key::DEL:
		return ImGuiKey_Delete;
	case cross::Key::HELP:
		return ImGuiKey_NamedKey_BEGIN;	//does not know this key
	case cross::Key::NUM_0:
		return ImGuiKey_0;
	case cross::Key::NUM_1:
		return ImGuiKey_1;
	case cross::Key::NUM_2:
		return ImGuiKey_2;
	case cross::Key::NUM_3:
		return ImGuiKey_3;
	case cross::Key::NUM_4:
		return ImGuiKey_4;
	case cross::Key::NUM_5:
		return ImGuiKey_5;
	case cross::Key::NUM_6:
		return ImGuiKey_6;
	case cross::Key::NUM_7:
		return ImGuiKey_7;
	case cross::Key::NUM_8:
		return ImGuiKey_8;
	case cross::Key::NUM_9:
		return ImGuiKey_9;
	case cross::Key::A:
		return ImGuiKey_A;
	case cross::Key::B:
		return ImGuiKey_B;
	case cross::Key::C:
		return ImGuiKey_C;
	case cross::Key::D:
		return ImGuiKey_D;
	case cross::Key::E:
		return ImGuiKey_E;
	case cross::Key::F:
		return ImGuiKey_F;
	case cross::Key::G:
		return ImGuiKey_G;
	case cross::Key::H:
		return ImGuiKey_H;
	case cross::Key::I:
		return ImGuiKey_I;
	case cross::Key::J:
		return ImGuiKey_J;
	case cross::Key::K:
		return ImGuiKey_K;
	case cross::Key::L:
		return ImGuiKey_L;
	case cross::Key::M:
		return ImGuiKey_M;
	case cross::Key::N:
		return ImGuiKey_N;
	case cross::Key::O:
		return ImGuiKey_O;
	case cross::Key::P:
		return ImGuiKey_P;
	case cross::Key::Q:
		return ImGuiKey_Q;
	case cross::Key::R:
		return ImGuiKey_R;
	case cross::Key::S:
		return ImGuiKey_S;
	case cross::Key::T:
		return ImGuiKey_T;
	case cross::Key::U:
		return ImGuiKey_U;
	case cross::Key::V:
		return ImGuiKey_V;
	case cross::Key::W:
		return ImGuiKey_W;
	case cross::Key::X:
		return ImGuiKey_X;
	case cross::Key::Y:
		return ImGuiKey_Y;
	case cross::Key::Z:
		return ImGuiKey_Z;
	case cross::Key::NUMPAD_0:
		return ImGuiKey_Keypad0;
	case cross::Key::NUMPAD_1:
		return ImGuiKey_Keypad1;
	case cross::Key::NUMPAD_2:
		return ImGuiKey_Keypad2;
	case cross::Key::NUMPAD_3:
		return ImGuiKey_Keypad3;
	case cross::Key::NUMPAD_4:
		return ImGuiKey_Keypad4;
	case cross::Key::NUMPAD_5:
		return ImGuiKey_Keypad5;
	case cross::Key::NUMPAD_6:
		return ImGuiKey_Keypad6;
	case cross::Key::NUMPAD_7:
		return ImGuiKey_Keypad7;
	case cross::Key::NUMPAD_8:
		return ImGuiKey_Keypad8;
	case cross::Key::NUMPAD_9:
		return ImGuiKey_Keypad9;
	case cross::Key::MULTIPLY:
		return ImGuiKey_KeypadMultiply;
	case cross::Key::ADD:
		return ImGuiKey_KeypadAdd;
	case cross::Key::SEPARATOR:
		return ImGuiKey_NamedKey_BEGIN;	//does not know this key
	case cross::Key::SUBTRACT:
		return ImGuiKey_KeypadSubtract;
	case cross::Key::DECIMAL:
		return ImGuiKey_KeypadDecimal;
	case cross::Key::DIVIDE:
		return ImGuiKey_KeypadDivide;
	case cross::Key::F1:
		return ImGuiKey_F1;
	case cross::Key::F2:
		return ImGuiKey_F2;
	case cross::Key::F3:
		return ImGuiKey_F3;
	case cross::Key::F4:
		return ImGuiKey_F4;
	case cross::Key::F5:
		return ImGuiKey_F5;
	case cross::Key::F6:
		return ImGuiKey_F6;
	case cross::Key::F7:
		return ImGuiKey_F7;
	case cross::Key::F8:
		return ImGuiKey_F8;
	case cross::Key::F9:
		return ImGuiKey_F9;
	case cross::Key::F10:
		return ImGuiKey_F10;
	case cross::Key::F11:
		return ImGuiKey_F11;
	case cross::Key::F12:
		return ImGuiKey_F12;
	case cross::Key::NUMLOC:
		return ImGuiKey_NumLock;
	case cross::Key::SCROLLLOCK:
		return ImGuiKey_ScrollLock;
	case cross::Key::SEMICOLON:
		return ImGuiKey_Semicolon;
	case cross::Key::PLUS:
		return ImGuiKey_Equal;
	case cross::Key::COMMA:
		return ImGuiKey_Comma;
	case cross::Key::MINUS:
		return ImGuiKey_Minus;
	case cross::Key::DOT:
		return ImGuiKey_Period;
	case cross::Key::SLASH:
		return ImGuiKey_Slash;
	case cross::Key::TILDE:
		return ImGuiKey_GraveAccent;
	case cross::Key::L_SQUARE_BRACKET:
		return ImGuiKey_LeftBracket;
	case cross::Key::BACKSLASH:
		return ImGuiKey_Backslash;
	case cross::Key::R_SQUARE_BRACKET:
		return ImGuiKey_RightBracket;
	case cross::Key::APOSTROPHE:
		return ImGuiKey_Apostrophe;
	case cross::Key::BACK:
		return ImGuiKey_NamedKey_BEGIN;	//does not know this key
	case cross::Key::MAX_KEY_NUM:
		return ImGuiKey_NamedKey_COUNT;
	default:
		return ImGuiKey_None;
	}
}

String Demo::GetCompactSize(U64 bytes) {
	if(bytes < 10 * 1024) {
		return String(bytes) + " b";
	} else if(bytes < 10 * 1024 * 1024) {
		return String((float)bytes / 1024.f, "%.2f", 40) + " kb";
	} else {
		return String((float)bytes / (1024.f * 1024.f), "%.2f", 40) + " mb";
	}
}

const char* Demo::GetClipboardString(ImGuiContext* context) {
	demo->clipboard = os->GetClipboard();
	return demo->clipboard;
}

void Demo::SetClipboardString(ImGuiContext* context, const char* data) {
	os->SetClipboard(data);
	demo->clipboard = data;
}

Game* CrossMain() {
	return CREATE Demo();
}

void Demo::Start() {
	Game::Start();
	os->LogIt("Demo::Start()");
	demo = (Demo*)game;

	GetComponentFactory()->Register<AnimatedCameraController>("AnimatedCameraController");

	input->ActionDown.Connect(this, &Demo::ActionDownHandle);
	input->ActionMove.Connect(this, &Demo::ActionMoveHandle);
	input->ActionUp.Connect(this, &Demo::ActionUpHandle);
	input->KeyPressed.Connect(this, &Demo::KeyPressed);
	input->KeyReleased.Connect(this, &Demo::KeyReleased);
	input->CharEnter.Connect(this, &Demo::CharEnter);
	input->Scroll.Connect(this, &Demo::WheelRoll);

	imgui_filename = os->DataPath() + imgui_filename;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = imgui_filename;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
	platformIO.Platform_GetClipboardTextFn = GetClipboardString;
	platformIO.Platform_SetClipboardTextFn = SetClipboardString;

    CreateUIShaders();
	CreateFontsTexture();

	ImGui::LoadStyle();

	if(os->IsMobile()) {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 5 * os->GetScreenScale();
		style.ScrollbarSize = 20 * os->GetScreenScale();
		style.ScrollbarRounding = 5 * os->GetScreenScale();
		style.ItemSpacing.x = 5 * os->GetScreenScale();
		style.ItemSpacing.y = 5 * os->GetScreenScale();
		style.WindowPadding.x = 5 * os->GetScreenScale();
		style.WindowPadding.y = 5 * os->GetScreenScale();
		style.FramePadding.x = 5 * os->GetScreenScale();
		style.FramePadding.y = 5 * os->GetScreenScale();
		style.IndentSpacing = 25 * os->GetScreenScale();
		style.GrabRounding = 5 * os->GetScreenScale();
		style.GrabMinSize = 20 * os->GetScreenScale();
	}

	menu = CREATE MenuBar();
	launch_view = CREATE LaunchView();

	ToMain();

	RunTest();
}

void Demo::Stop() {
	os->LogIt("Demo::Stop()");
	delete launch_view;
	delete menu;
	SAFE(glDeleteBuffers(1, &vertex_buffer));
	SAFE(glDeleteBuffers(1, &index_buffer));
	delete font_texture;
	delete ui_shader;
	Game::Stop();
}

void Demo::PreUpdate(float sec) {
	ImGuiIO& io = ImGui::GetIO();
	// Setup display size (every frame to accommodate for window resizing)
	int windowWidth = os->GetWindowWidth();
	int windowHeight = os->GetWindowHeight();
	io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
	io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
	io.DeltaTime = sec;

#if defined(WIN)
	io.MousePos = ImVec2(input->MousePosition.x, input->MousePosition.y);
#else
	io.MousePos = ImVec2(action_pos.x, (float)os->GetWindowHeight() - action_pos.y);
#endif
	for(S32 i = 0; i < MaxInputActions; i++) {
		io.MouseDown[i] = actions_down[i];
	}

	io.MouseWheel = mouse_wheel;
	mouse_wheel = 0.f;

	// Start the frame
	ImGui::NewFrame();
}

void Demo::Update(float sec) {
	Game::Update(sec);

	menu->Update(sec);
	menu->ShowMenu();
	launch_view->Run(sec);

	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	RenderUI(drawData);
}

void Demo::PostUpdate(float sec) {
	for(int i = 0; i < MaxInputActions; i++) {
		if(actions_up[i]) {
			actions_down[i] = false;
			actions_up[i] = false;
		}
	}
}

void Demo::SetScreen(Screen* screen) {
	launch_view->Hide();
	Game::SetScreen(screen);
}

void Demo::ToMain() {
	Screen* mainScreen = CREATE Screen();
	mainScreen->SetName("Main");
	mainScreen->SetBackground(Color(0.3f));
	SetScreen(mainScreen);
	launch_view->Show();
}

LaunchView* Demo::GetLaunchView() {
	return launch_view;
}

MenuBar* Demo::GetMenuBar() {
	return menu;
}

bool Demo::CreateUIShaders() {
	ui_shader = gfx->LoadShader("Engine/Shaders/UI.sha");
	ui_shader->Compile();

	SAFE(glGenBuffers(1, &vertex_buffer));
	SAFE(glGenBuffers(1, &index_buffer));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
	SAFE(glEnableVertexAttribArray(ui_shader->aPosition));
	SAFE(glEnableVertexAttribArray(ui_shader->aTexCoords));
	SAFE(glEnableVertexAttribArray(ui_shader->aColor));

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	SAFE(glVertexAttribPointer(ui_shader->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos)));
	SAFE(glVertexAttribPointer(ui_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv)));
	SAFE(glVertexAttribPointer(ui_shader->aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col)));
#undef OFFSETOF
	return true;
}

bool Demo::CreateFontsTexture() {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();

	ImFontConfig fontConfig;
	float fontScale = Round(os->GetScreenScale());
	CROSS_ASSERT(fontScale != 0, "Font scale == 0");
	fontConfig.SizePixels = DEFAULT_FONT_SIZE * fontScale;
	String fontName = "ProggyClean.ttf, " + String((int)fontConfig.SizePixels) + "px";
	memcpy(fontConfig.Name, fontName.ToCStr(), fontName.Length() + 1);
	small_font = io.Fonts->AddFontDefault(&fontConfig);

	fontConfig.SizePixels = DEFAULT_FONT_SIZE * fontScale * 1.5f;
	fontName.Clear();
	fontName = "ProggyClean.ttf, " + String((int)fontConfig.SizePixels) + "px";
	memcpy(fontConfig.Name, fontName.ToCStr(), fontName.Length() + 1);
	normal_font = io.Fonts->AddFontDefault(&fontConfig);

	fontConfig.SizePixels = DEFAULT_FONT_SIZE * fontScale * 2.f;
	fontName.Clear();
	fontName = "ProggyClean.ttf, " + String((int)fontConfig.SizePixels) + "px";
	memcpy(fontConfig.Name, fontName.ToCStr(), fontName.Length() + 1);
	big_font = io.Fonts->AddFontDefault(&fontConfig);

	unsigned char* pixels;
	int width, height;

	// Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	os->LogIt("Creating font texture(#x#)", width, height);
	font_texture = CREATE Texture();
	font_texture->Create(pixels, 4, width, height,
		Texture::Filter::LINEAR,
		Texture::Compression::NONE,
		Texture::TilingMode::CLAMP_TO_EDGE, false);
	// Store our identifier
	io.Fonts->TexID = (ImTextureID)font_texture->GetID();
	return true;
}

void Demo::RenderUI(ImDrawData* draw_data) {
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if(fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	SAFE(glActiveTexture(GL_TEXTURE0));
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendEquation(GL_FUNC_ADD));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
	SAFE(glEnable(GL_SCISSOR_TEST));

	ui_shader->Use();

	Matrix projection = Matrix::CreateOrthogonalProjection(0, io.DisplaySize.x, io.DisplaySize.y, 0, 1, -1);
	SAFE(glUniformMatrix4fv(ui_shader->uMVP, 1, GL_FALSE, projection.GetTransposed().GetData()));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
	SAFE(glEnableVertexAttribArray(ui_shader->aPosition));
	SAFE(glEnableVertexAttribArray(ui_shader->aTexCoords));
	SAFE(glEnableVertexAttribArray(ui_shader->aColor));

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	SAFE(glVertexAttribPointer(ui_shader->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos)));
	SAFE(glVertexAttribPointer(ui_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv)));
	SAFE(glVertexAttribPointer(ui_shader->aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col)));
#undef OFFSETOF
	for(int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		SAFE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		SAFE(glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW));

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
		SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW));

		for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if(pcmd->UserCallback) {
				pcmd->UserCallback(cmd_list, pcmd);
			} else {
				SAFE(glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId));
				SAFE(glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)));
				SAFE(glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset));
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_SCISSOR_TEST));

	static GLsizei scissorBox[4];
	scissorBox[0] = 0;
	scissorBox[1] = 0;
	scissorBox[2] = os->GetWindowWidth();
	scissorBox[3] = os->GetWindowHeight();
	SAFE(glScissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]));
}

void Demo::ActionDownHandle(Input::Action action) {
	actions_down[action.id] = true;
	action_pos = action.pos;
}

void Demo::ActionMoveHandle(Input::Action action) {
	action_pos = action.pos;
}

void Demo::ActionUpHandle(Input::Action action) {
	actions_up[action.id] = true;
	action_pos = action.pos;
}

void Demo::KeyPressed(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(FromCrossKeyToImgui(key), true);
	if(key == Key::CONTROL) {
		io.KeyCtrl = true;
	}
	if(key == Key::SHIFT) {
		io.KeyShift = true;
	}
	if(key == Key::ALT) {
		io.KeyAlt = true;
	}
	if(key == Key::COMMAND) {
		io.KeySuper = true;
	}
}

void Demo::KeyReleased(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(FromCrossKeyToImgui(key), false);
	if(key == Key::CONTROL) {
		io.KeyCtrl = false;
	}
	if(key == Key::SHIFT) {
		io.KeyShift = false;
	}
	if(key == Key::ALT) {
		io.KeyAlt = false;
	}
	if (key == Key::COMMAND) {
		io.KeySuper = false;
	}
}

void Demo::CharEnter(char c) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}

void Demo::WheelRoll(float delta) {
	mouse_wheel += delta;
}
