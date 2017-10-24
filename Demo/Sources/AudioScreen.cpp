#include "AudioScreen.h"
#include "Demo.h"
#include "Utils/Sprite.h"
#include "Utils/ToggleButton.h"
#include "Audio.h"
#include "Sound.h"

using namespace cross;

void AudioScreen::Start(){
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
	is_bck_playing = false;

	SetBackground(Color(0.25f, 0.25f, 0.25f));
	button_sprite = demo->GetCommonSprite("ButtonTemplate.png");
	button_sprite_pressed = demo->GetCommonSprite("ButtonTemplatePressed.png");
	Button* soundBtn = new Button();
	soundBtn->SetImages(button_sprite->Clone(), nullptr);
	ToggleButton* loopBtn = new ToggleButton(button_sprite->Clone(), button_sprite_pressed->Clone());
	ToggleButton* streamBtn = new ToggleButton(button_sprite->Clone(), button_sprite_pressed->Clone());
	soundBtn->Clicked.Connect(this, &AudioScreen::OnSoundButtonClick);
	loopBtn->Clicked.Connect(this, &AudioScreen::OnLoopButttonClick);
	streamBtn->Clicked.Connect(this, &AudioScreen::OnStreamButtonClick);

	bck_music = audio->LoadSound("Audio/Music.mp3", true, true);
	jaguar = audio->LoadSound("Audio/Jaguar.wav", false, false);
	truck = audio->LoadSound("Audio/Truck.wav", true, false);
}

void AudioScreen::Stop(){
	delete bck_music;
	delete jaguar;
	delete truck;
}

void AudioScreen::Update(float sec){

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
