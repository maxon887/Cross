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
#include "MainScreen.h"
#include "Game.h"
#include "GraphicsGL.h"
#include "System.h"
#include "UIScreen.h"
#include "Texture.h"
#include "Graphics3D/Simple/TriangleScene.h"

#include "Libs/ImGui/imgui.h"

void MainScreen::Start(){
	Screen::Start();
	SetBackground(Color(0.3f));

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig fontConfig;
	fontConfig.SizePixels = system->GetScreenDPI() / DEFAULT_SCREEN_DPI * DEFAULT_FONT_SIZE * 2;
	font_big = io.Fonts->AddFontDefault(&fontConfig);

	unsigned char* pixels;
	int width, height;
#ifdef USE_FREETYPE
	ImGuiFreeType::BuildFontAtlas(io.Fonts);
#endif
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
	big_font_texture = new Texture();
	big_font_texture->Create(	pixels, 4, width, height,
								Texture::Filter::LINEAR,
								Texture::Compression::NONE,
								Texture::TilingMode::CLAMP_TO_EDGE, false);
	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)big_font_texture->GetID();
}

void MainScreen::Stop(){
	delete big_font_texture;
	Screen::Stop();
}

void MainScreen::Update(float sec){
	Screen::Update(sec);
	ImGui::PushFont(font_big);
	ImVec2 textSize = ImGui::CalcTextSize("Graphics");

	ImGui::SetNextWindowSize(ImVec2(textSize.x * 2.f, textSize.y * 2.f * 2.f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(GetWidth() / 2.f, GetHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Demo");

	if(ImGui::CollapsingHeader("Graphics")) {
		if(ImGui::TreeNode("Simple")) {
			ImGui::MenuButton("Triangle");
			ImGui::MenuButton("Solid Model");
			ImGui::MenuButton("Textured Model");
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Light")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Maps")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Misc")) {
			ImGui::TreePop();
		}
	}

	if(ImGui::Button("GUI", ImVec2(-1, 0))) {
		game->SetScreen(new UIScreen());
	}
	ImGui::End();
	ImGui::PopFont();
}
