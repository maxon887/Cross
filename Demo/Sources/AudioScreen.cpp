#include "AudioScreen.h"
#include "Demo.h"
#include "UI/MenuBar.h"
#include "Sound.h"
#include "System.h"

#include "ThirdParty/ImGui/imgui.h"

using namespace cross;

void AudioScreen::Start(){
	Screen::Start();
	bck_music = new Sound("Audio/Music.mp3", true, true);
	jaguar = new Sound("Audio/Jaguar.wav", false, false);
	truck = new Sound("Audio/Truck.wav", true, false);
}

void AudioScreen::Stop(){
	delete bck_music;
	delete jaguar;
	delete truck;
	Screen::Stop();
}

void AudioScreen::Update(float sec){
	Screen::Update(sec);

	ImGui::PushFont(demo->big_font);

	if(!system->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2(system->GetWindowWidth() / 3.f, system->GetWindowHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() / 2.f, system->GetWindowHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Audio");
	} else {
		ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth(), system->GetWindowHeight() - demo->GetMenuBar()->GetHeight()));
		ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
		ImGui::Begin("Audio", 0,	ImGuiWindowFlags_NoCollapse |
									ImGuiWindowFlags_NoMove |
									ImGuiWindowFlags_NoTitleBar |
									ImGuiWindowFlags_NoResize |
									ImGuiWindowFlags_NoBringToFrontOnFocus);
	}

	//ImGui::PushFont(font);
	ImGui::Text("Cross++ use FMOD for sound playing. Click button for play an example.");
	//ImGui::PopFont();

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
