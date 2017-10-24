#pragma once
#include "Screen.h"
#include "Utils/Sprite.h"

using namespace cross;

class Menu;

class AudioScreen :	public Screen{
public:
	void Start();
	void Stop();
	void Update(float sec);

private:
	Sprite* button_sprite;
	Sprite* button_sprite_pressed;
	Sound* bck_music;
	Sound* jaguar;
	Sound* truck;
	bool is_bck_playing;

	void OnSoundButtonClick();
	void OnLoopButttonClick();
	void OnStreamButtonClick();
};

