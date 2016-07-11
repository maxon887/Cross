#include "AudioScreen.h"
#include "Audio.h"
#include "Graphics2D.h"
#include "Demo.h"
#include "Config.h"
#include "Sprite.h"
#include "Camera2D.h"

using namespace cross;

void AudioScreen::Start(){
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
	is_bck_playing = false;

	camera = new Camera2D();
	camera->SetViewWidth(1600.f);
	gfx2D->SetCamera(camera);

	SetBackground(Color(0.25f, 0.25f, 0.25f));
	button_sprite = demo->GetCommonSprite("DefaultButton.png");
	button_sprite->SetScale(1.35f);
	button_sprite_pressed = demo->GetCommonSprite("DefaultButtonPressed.png");
	button_sprite_pressed->SetScale(1.35f);
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

	bck_music = new Audio("bck_music.mp3", true, true);
	jaguar = new Audio("Jaguar.wav", false, false);
	truck = new Audio("Truck.wav", true, false);
}

void AudioScreen::Stop(){
	delete bck_music;
	delete jaguar;
	delete truck;
	delete audio_menu;
	delete camera;
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
