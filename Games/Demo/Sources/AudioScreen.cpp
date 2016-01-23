#include "AudioScreen.h"
#include "Audio.h"
#include "Graphics2D.h"
#include "Game.h"
#include "Config.h"

using namespace cross;

AudioScreen::~AudioScreen(void){
	delete bck_music;
	delete jaguar;
	delete truck;
	delete yellow_sound_btn;
	delete blue_sound_btn;
	delete music_btn;
}

void AudioScreen::Start(){
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
	going_back = false;
	input->KeyPressed += MakeDelegate(this, &AudioScreen::OnKeyPressed);
	Sprite* on = gfx2D->LoadImage("MusicOn.png");
	Sprite* off = gfx2D->LoadImage("MusicOff.png");
	Vector2D pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 4 * 3;
	music_btn = new ToggleButton(pos, on, off);
	music_btn->Clicked += MakeDelegate(this, &AudioScreen::MusicOnClick);
	bck_music = new Audio("Eskimo.mp3", true, true);
	bool musicState = config->LoadBool("MUSIC_STATE", true);
	music_btn->SetState(musicState);
	if(musicState){
		bck_music->Play();
		song_started = true;
	}else{
		song_started = false;
	}

	Sprite* yellow_img = gfx2D->LoadImage("SoundButtonYellow.png");
	Sprite* blue_img = gfx2D->LoadImage("SoundButtonBlue.png");
	//Sprite* gray_img = graphics->LoadImage("SoundButtonGray.png");
	yellow_sound_btn = new Button(yellow_img, NULL);
	pos.x = yellow_sound_btn->GetWidth() / 2;
	pos.y = game->GetHeight() - yellow_sound_btn->GetHeight() / 2;
	yellow_sound_btn->SetLocation(pos);
	yellow_sound_btn->Clicked += MakeDelegate(this, &AudioScreen::OnYellowClick);
	blue_sound_btn = new Button(blue_img, NULL);
	pos.x = game->GetWidth() - blue_sound_btn->GetWidth() / 2;
	pos.y = game->GetHeight() - blue_sound_btn->GetHeight() / 2;
	blue_sound_btn->SetLocation(pos);
	blue_sound_btn->Clicked += MakeDelegate(this, &AudioScreen::OnBlueClick);

	jaguar = new Audio("Jaguar.wav", false, false);
	truck = new Audio("Truck.wav", true, false);
}

void AudioScreen::Update(float sec){
	gfx2D->Clear();
	yellow_sound_btn->Update();
	blue_sound_btn->Update();
	music_btn->Update();

	if(going_back){
		game->SetScreen(game->GetStartScreen());
	}
}


void AudioScreen::OnYellowClick(){
	jaguar->Play();
}

void AudioScreen::OnBlueClick(){
	if(truck->IsPlaying()){
		truck->Stop();
	}else{
		truck->Play();
	}
}

void AudioScreen::MusicOnClick(){
	if(music_btn->GetState()){
		if(song_started)
			bck_music->Resume();
		else
			bck_music->Play();
	}
	else
		bck_music->Pause();
	config->SaveBool("MUSIC_STATE", music_btn->GetState());
}

void AudioScreen::OnKeyPressed(Key key){
	switch (key) {
	case cross::Key::UNDEFINED:
		break;
	case cross::Key::PAUSE:
		break;
	case cross::Key::BACK:
		break;
	case cross::Key::OPTIONS:
		break;
	case cross::Key::UP:
		break;
	case cross::Key::DOWN:
		break;
	case cross::Key::RIGHT:
		break;
	case cross::Key::LEFT:
		break;
	case cross::Key::ENTER:
		break;
	case cross::Key::SPACE:
		break;
	case cross::Key::SHIFT:
		break;
	case cross::Key::ESCAPE:
		going_back = true;
		break;
	default:
		break;
	}
}