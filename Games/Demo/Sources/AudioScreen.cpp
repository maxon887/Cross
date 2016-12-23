#include "AudioScreen.h"
#include "Demo.h"
#include "Sprite.h"
#include "Utils/ToggleButton.h"
#include "Menu.h"
#include "Audio.h"

using namespace cross;

void AudioScreen::Start(){
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
	is_bck_playing = false;

	SetBackground(Color(0.25f, 0.25f, 0.25f));
	button_sprite = demo->GetCommonSprite("ButtonTemplate.png");
	button_sprite_pressed = demo->GetCommonSprite("ButtonTemplatePressed.png");
	audio_menu = new Menu(true);
	Button* soundBtn = new Button("Sound");
	soundBtn->SetImages(button_sprite->Clone(), nullptr);
	ToggleButton* loopBtn = new ToggleButton(button_sprite->Clone(), button_sprite_pressed->Clone());
	loopBtn->SetText("Loop");
	ToggleButton* streamBtn = new ToggleButton(button_sprite->Clone(), button_sprite_pressed->Clone());
	streamBtn->SetText("Stream");
	soundBtn->Clicked += MakeDelegate(this, &AudioScreen::OnSoundButtonClick);
	loopBtn->Clicked += MakeDelegate(this, &AudioScreen::OnLoopButttonClick);
	streamBtn->Clicked += MakeDelegate(this, &AudioScreen::OnStreamButtonClick);
	audio_menu->AddButton(soundBtn);
	audio_menu->AddButton(loopBtn);
	audio_menu->AddButton(streamBtn);

	bck_music = audio->LoadSound("Audio/Music.mp3", true, true);
	jaguar = audio->LoadSound("Audio/Jaguar.wav", false, false);
	truck = audio->LoadSound("Audio/Truck.wav", true, false);
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
