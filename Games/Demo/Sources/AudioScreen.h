#pragma once
#include "Screen.h"

using namespace cross;

class Menu;

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

