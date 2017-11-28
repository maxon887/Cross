/*	Copyright © 2015 Lukyanau Maksim

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
#include "MainScreen.h"
#include "System.h"
#include "Config.h"
#include "Shaders/Shader.h"
#include "UI/MenuBar.h"
#include "Utils/Debugger.h"
#ifdef WIN
#	include "Platform/Windows/WINSystem.h"
#endif

#if defined(USE_FREETYPE) && defined(WIN)
#	pragma comment(lib, "freetype.lib")
#endif

#include "ThirdParty/ImGui/imgui.h"
#ifdef USE_FREETYPE
#	include "ThirdParty/ImGui/imgui_freetype.h"
#endif

#define DEFAULT_FONT_SIZE 13.f

Demo* demo = NULL;

const char* Demo::GetClipboardString(void* userData) {
	demo->clipboard = system->GetClipboard();
	return demo->clipboard.c_str();
}

void Demo::Start() {
	Game::Start();
	system->LogIt("Demo::Start()");
	demo = (Demo*)game;

	input->ActionDown.Connect(this, &Demo::ActionDownHandle);
	input->ActionMove.Connect(this, &Demo::ActionMoveHandle);
	input->ActionUp.Connect(this, &Demo::ActionUpHandle);
	input->KeyPressed.Connect(this, &Demo::KeyPressed);
	input->KeyReleased.Connect(this, &Demo::KeyReleased);
	input->CharEnter.Connect(this, &Demo::CharEnter);
	input->MouseWheelRoll.Connect(this, &Demo::WheelRoll);

	ImGuiIO& io = ImGui::GetIO();
	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = (int)Key::TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = (int)Key::LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = (int)Key::RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = (int)Key::UP;
	io.KeyMap[ImGuiKey_DownArrow] = (int)Key::DOWN;
	io.KeyMap[ImGuiKey_PageUp] = (int)Key::PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = (int)Key::PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = (int)Key::HOME;
	io.KeyMap[ImGuiKey_End] = (int)Key::END;
	io.KeyMap[ImGuiKey_Delete] = (int)Key::DEL;
	io.KeyMap[ImGuiKey_Backspace] = (int)Key::BACK;
	io.KeyMap[ImGuiKey_Escape] = (int)Key::ESCAPE;
	io.KeyMap[ImGuiKey_A] = (int)Key::A;
	io.KeyMap[ImGuiKey_C] = (int)Key::C;
	io.KeyMap[ImGuiKey_V] = (int)Key::V;
	io.KeyMap[ImGuiKey_X] = (int)Key::X;
	io.KeyMap[ImGuiKey_Y] = (int)Key::Y;
	io.KeyMap[ImGuiKey_Z] = (int)Key::Z;

	//io.SetClipboardTextFn = ImGui_ImplGlfwGL3_SetClipboardText;
	io.GetClipboardTextFn = GetClipboardString;
	//io.ClipboardUserData = g_Window;
#ifdef _WIN32
	WINSystem* winSys = (WINSystem*)system;
	io.ImeWindowHandle = winSys->GetHWND();
#endif

	CreateDeviceObjects();
	CreateFontsTexture();


	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::UseStyle(config->GetInt("IMGUI_STYLE", 0));

	if(system->IsMobile()) {
		style.WindowRounding = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.ScrollbarSize = 20 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.ScrollbarRounding = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.ItemSpacing.x = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.ItemSpacing.y = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.WindowPadding.x = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.WindowPadding.y = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.FramePadding.x = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.FramePadding.y = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.IndentSpacing = 25 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.GrabRounding = 5 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
		style.GrabMinSize = 20 * system->GetScreenDPI() / DEFAULT_SCREEN_DPI;
	}

	menu = new MenuBar();
}

void Demo::Stop() {
	system->LogIt("Demo::Stop()");
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
	int windowWidth = system->GetWindowWidth();
	int windowHeight = system->GetWindowHeight();
	io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
	io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
	io.DeltaTime = sec;

#if defined(WIN)
	io.MousePos = ImVec2(input->MousePosition.x, input->MousePosition.y);
#else
	io.MousePos = ImVec2(action_pos.x, system->GetWindowHeight() - action_pos.y);
#endif
	for(U32 i = 0; i < 5; i++) {
		io.MouseDown[i] = actions[i];
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

	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	RenderUI(drawData);
}

Screen* Demo::GetStartScreen() {
	return new MainScreen();
}

MenuBar* Demo::GetMenuBar() {
	return menu;
}

bool Demo::CreateDeviceObjects() {
	ui_shader = new Shader();
	ui_shader->Load("Engine/Shaders/UI.sha");
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
	float fontScale = (float)(int)(system->GetScreenDPI() / DEFAULT_SCREEN_DPI + 0.5f);
	CROSS_ASSERT(fontScale != 0, "Font scale == 0");
	fontConfig.SizePixels = DEFAULT_FONT_SIZE * fontScale;
	normal_font = io.Fonts->AddFontDefault(&fontConfig);

	fontConfig.SizePixels = DEFAULT_FONT_SIZE * fontScale * 2.f;
	big_font = io.Fonts->AddFontDefault(&fontConfig);

	unsigned char* pixels;
	int width, height;
#ifdef USE_FREETYPE
	ImGuiFreeType::BuildFontAtlas(io.Fonts);
#endif
	// Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	system->LogIt("Creating font texture(%dx%d)", width, height);
	font_texture = new Texture();
	font_texture->Create(pixels, 4, width, height,
		Texture::Filter::LINEAR,
		Texture::Compression::NONE,
		Texture::TilingMode::CLAMP_TO_EDGE, false);
	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)font_texture->GetID();
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
	scissorBox[2] = system->GetWindowWidth();
	scissorBox[3] = system->GetWindowHeight();
	SAFE(glScissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]));
}

void Demo::ActionDownHandle(Input::Action action) {
	actions[action.id] = true;
	action_pos = action.pos;
}

void Demo::ActionMoveHandle(Input::Action action) {
	action_pos = action.pos;
}

void Demo::ActionUpHandle(Input::Action action) {
	actions[action.id] = false;
	action_pos = action.pos;
}

void Demo::KeyPressed(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[(int)key] = true;
	if(key == Key::CONTROL) {
		io.KeyCtrl = true;
	}
	if(key == Key::SHIFT) {
		io.KeyShift = true;
	}
	if(key == Key::ALT) {
		io.KeyAlt = true;
	}

	if(key == Key::ESCAPE || key == Key::BACK) {
		SetScreen(GetStartScreen());
	}
}

void Demo::KeyReleased(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[(int)key] = false;
	if(key == Key::CONTROL) {
		io.KeyCtrl = false;
	}
	if(key == Key::SHIFT) {
		io.KeyShift = false;
	}
	if(key == Key::ALT) {
		io.KeyAlt = false;
	}
}

void Demo::CharEnter(char c) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}

void Demo::WheelRoll(float delta) {
	mouse_wheel += delta / 120.f;	// Use fractional mouse wheel, 1.0 unit 5 lines.
}
