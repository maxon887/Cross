#include "AudioScreen.h"
#include "Audio.h"
#include "Graphics2D.h"
#include "Game.h"
#include "Config.h"
#include "Sprite.h"

using namespace cross;

void AudioScreen::Start(){
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
	is_bck_playing = false;

	gfx2D->SetClearColor(Color(0.25f, 0.25f, 0.25f));
	button_sprite = gfx2D->LoadImage("DefaultButton.png");
	audio_menu = new Menu();
	Button* soundBtn = new Button("Sound");
	soundBtn->SetImages(button_sprite->Clone(), nullptr);
	Button* loopBtn = new Button("Loop");
	loopBtn->SetImages(button_sprite->Clone(), nullptr);
	Button* streamBtn = new Button("Stream");
	streamBtn->SetImages(button_sprite->Clone(), nullptr);
	soundBtn->Clicked += MakeDelegate(this, &AudioScreen::OnSoundButtonClick);
	loopBtn->Clicked += MakeDelegate(this, &AudioScreen::OnLoopButttonClick);
	streamBtn->Clicked += MakeDelegate(this, &AudioScreen::OnStreamButtonClick);
	audio_menu->AddButton(soundBtn);
	audio_menu->AddButton(loopBtn);
	audio_menu->AddButton(streamBtn);

	bck_music = new Audio("bck_music.mp3", true, true);
	jaguar = new Audio("Jaguar.wav", false, false);
	truck = new Audio("Truck.wav", true, false);
}

void AudioScreen::Stop(){
	delete bck_music;
	delete jaguar;
	delete truck;
	delete audio_menu;
}

void AudioScreen::Update(float sec){
	audio_menu->Update(sec);

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}

void AudioScreen::OnSoundButtonClick(){
	jaguar->Play();
}

void AudioScreen::OnLoopButttonClick(){
	if(truck->IsPlaying()) {
		truck->Stop();
	} else {
		truck->Play();
	}
}

void AudioScreen::OnStreamButtonClick(){
	if(is_bck_playing){
		bck_music->Pause();
		is_bck_playing = false;
	}else{
		bck_music->Play();
		is_bck_playing = true;
	}
}
