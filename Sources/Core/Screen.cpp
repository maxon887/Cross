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
#include "Screen.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "System.h"
#include "GraphicsGL.h"
#include "Game.h"
#include "Shaders/Shader.h"

#include "Libs/ImGui/imgui.h"

using namespace cross;

void Screen::Start(){
	camera2D = new Camera2D();
    down_del = input->ActionDown.Connect(this, &Screen::ActionDownHandle);
    move_del = input->ActionMove.Connect(this, &Screen::ActionMoveHandle);
    up_del = input->ActionUp.Connect(this, &Screen::ActionUpHandle);
	key_pressed_del = input->KeyPressed.Connect(this, &Screen::KeyPressed);
	key_released_del = input->KeyReleased.Connect(this, &Screen::KeyReleased);
	char_enter_del = input->CharEnter.Connect(this, &Screen::CharEnter);
	wheel_roll = input->MouseWheelRoll.Connect(this, &Screen::WheelRoll);

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
	//io.GetClipboardTextFn = ImGui_ImplGlfwGL3_GetClipboardText;
	//io.ClipboardUserData = g_Window;
#ifdef _WIN32
	//WINSystem* winSys = (WINSystem*)system;
	//io.ImeWindowHandle = winSys->GetHWND();
#endif
}

void Screen::Stop(){
    input->ActionDown.Disconnect(down_del);
    input->ActionMove.Disconnect(move_del);
    input->ActionUp.Disconnect(up_del);
	input->KeyPressed.Disconnect(key_pressed_del);
	input->KeyReleased.Disconnect(key_released_del);
	input->CharEnter.Disconnect(char_enter_del);
	input->MouseWheelRoll.Disconnect(wheel_roll);
	delete camera2D;
	delete ui_shader;
}

void Screen::Update(float sec) {
	if(!font_texture) {
		CreateDeviceObjects();
	}

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int windowWidth = system->GetWindowWidth();
	int windowHeight = system->GetWindowHeight();
	io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
	io.DisplayFramebufferScale = ImVec2(windowWidth / GetWidth(), windowHeight / GetHeight());

	// Setup time step
	io.DeltaTime = sec;

	io.MousePos = ImVec2(input->MousePosition.x, input->MousePosition.y);
	for(U32 i = 0; i < actions.size(); i++) {
		io.MouseDown[i] = actions[i];
	}
	if(actions[0]) {	//for mobile input
		io.MousePos = ImVec2(action_pos.x, GetHeight() - action_pos.y);
	}

	io.MouseWheel = mouse_wheel;
	mouse_wheel = 0.f;

	// Start the frame
	ImGui::NewFrame();
}

void Screen::LateUpdate(float sec){
	camera2D->Update(sec);
	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	RenderUI(drawData);
}

float Screen::GetWidth(){
	return camera2D->GetViewWidth();
}

float Screen::GetHeight(){
	return camera2D->GetViewHeight();
}

Camera2D* Screen::GetCamera() {
	return camera2D;
}

bool Screen::IsScene() const{
	return is_scene;
}

float Screen::GetScaleFactor(){
	return (float)system->GetWindowWidth() / GetWidth();
}

void Screen::SetBackground(const Color& c){
	glClearColor(c.R, c.G, c.B, 1.f);
}

void Screen::EnableInputs(bool enable){
	enable_inputs = enable;
}

void Screen::RenderUI(ImDrawData* draw_data) {
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if(fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);
	
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);

	glActiveTexture(GL_TEXTURE0);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendEquation(GL_FUNC_ADD));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
	SAFE(glEnable(GL_SCISSOR_TEST));
	
	// Setup viewport, orthographic projection matrix
	//glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
		{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
		{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	gfxGL->UseShader(ui_shader);
	//glUniform1i(texture_loc, 0);
	glUniformMatrix4fv(ui_shader->uMVP, 1, GL_FALSE, &ortho_projection[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(ui_shader->aPosition);
	glEnableVertexAttribArray(ui_shader->aTexCoords);
	glEnableVertexAttribArray(ui_shader->aColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(ui_shader->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(ui_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(ui_shader->aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	for(int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if(pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			} else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_SCISSOR_TEST));

	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

bool Screen::CreateDeviceObjects()
{
	ui_shader = new Shader("Engine/Shaders/UI.vtx", "Engine/Shaders/UI.fgm");
	ui_shader->Compile();

	SAFE(glGenBuffers(1, &vertex_buffer));
	glGenBuffers(1, &index_buffer);

	//glGenVertexArrays(1, &g_VaoHandle);
	//glBindVertexArray(g_VaoHandle);
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
	glEnableVertexAttribArray(ui_shader->aPosition);
	glEnableVertexAttribArray(ui_shader->aTexCoords);
	glEnableVertexAttribArray(ui_shader->aColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(ui_shader->aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(ui_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(ui_shader->aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	CreateFontsTexture();
	return true;
}

bool Screen::CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig font;
	font.SizePixels = system->GetScreenDPI() / 96.0f * 13.0f;
	io.Fonts->AddFontDefault(&font);
	font.SizePixels = system->GetScreenDPI() / 96.0f * 13.0f * 2.0f;
	io.Fonts->AddFontDefault(&font);


	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

	// Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &font_texture);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)font_texture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);

	return true;
}

void Screen::ActionDownHandle(Input::Action action){
	if(enable_inputs){
		actions[action.id] = true;
		action_pos = action.pos;
		ActionDown(action);
	}
}

void Screen::ActionMoveHandle(Input::Action action){
	if(enable_inputs){
		action_pos = action.pos;
		ActionMove(action);
	}
}

void Screen::ActionUpHandle(Input::Action action){
	if(enable_inputs){
		actions[action.id] = false;
		action_pos = action.pos;
		ActionUp(action);
	}
}

void Screen::KeyPressed(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[(int)key] = true;
}

void Screen::KeyReleased(Key key) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[(int)key] = false;
}

void Screen::CharEnter(char c) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}

void Screen::WheelRoll(float delta) {
	mouse_wheel += delta / 120.f;	// Use fractional mouse wheel, 1.0 unit 5 lines.
}