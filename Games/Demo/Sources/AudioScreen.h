#pragma once
#include "Screen.h"
#include "Utils/ToggleButton.h"
#include "Input.h"
#include "Menu.h"

using namespace cross;

class AudioScreen :	public Screen{
public:
	void Start();
	void Stop();
	void Update(float sec);

private:
	Menu* audio_menu;
	Sprite* button_sprite;
	Sprite* button_sprite_pressed;
	Audio* bck_music;
	Audio* jaguar;
	Audio* truck;
	bool is_bck_playing;

	void OnSoundButtonClick();
	void OnLoopButttonClick();
	void OnStreamButtonClick();
};

