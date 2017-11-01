#include "AudioScreen.h"
#include "Audio.h"
#include "Sound.h"
#include "System.h"

#include "Libs/ImGui/imgui.h"

using namespace cross;

void AudioScreen::Start(){
	Screen::Start();
	MenuBar::Init();
	bck_music = audio->LoadSound("Audio/Music.mp3", true, true);
	jaguar = audio->LoadSound("Audio/Jaguar.wav", false, false);
	truck = audio->LoadSound("Audio/Truck.wav", true, false);
}

void AudioScreen::Stop(){
	delete bck_music;
	delete jaguar;
	delete truck;
	MenuBar::Release();
	Screen::Stop();
}

void AudioScreen::Update(float sec){
	Screen::Update(sec);
	MenuBar::Update(sec);
	MenuBar::ShowMenu();

	ImGui::PushFont(font_big);

	if(!system->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2(GetWidth() / 3.f, GetHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(GetWidth() / 2.f, GetHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Audio");
	} else {
		ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight() - menu_height));
		ImGui::SetNextWindowPos(ImVec2(0, menu_height));
		ImGui::Begin("Audio", 0,	ImGuiWindowFlags_NoCollapse |
									ImGuiWindowFlags_NoMove |
									ImGuiWindowFlags_NoTitleBar |
									ImGuiWindowFlags_NoResize |
									ImGuiWindowFlags_NoBringToFrontOnFocus);
	}

	ImGui::PushFont(font);
	ImGui::Text("Cross++ use FMOD for sound playing. Click button for play an example.");
	ImGui::PopFont();

	if(ImGui::Button("Simple Sound", ImVec2(-1, 0))) {
		jaguar->Play();
	}
	if(ImGui::Button("Loop", ImVec2(-1, 0))) {
		if(truck->IsPlaying()) {
			truck->Stop();
		} else {
			truck->Play();
		}
	}
	if(ImGui::Button("Stream", ImVec2(-1, 0))) {
		if(is_bck_playing) {
			bck_music->Pause();
			is_bck_playing = false;
		} else {
			bck_music->Play();
			is_bck_playing = true;
		}
	}

	ImGui::End();

	ImGui::PopFont();
}
