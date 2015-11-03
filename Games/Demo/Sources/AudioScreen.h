#pragma once
#include "Screen.h"
#include "ToggleButton.h"

using namespace cross;

class AudioScreen :	public Screen{
public:
	AudioScreen(Game* game);
	~AudioScreen(void);
	void Start();
	void Update(float sec);
private:
	Button* yellow_sound_btn;
	Button* blue_sound_btn;
	ToggleButton* music_btn;
	Audio* bck_music;
	Audio* jaguar;
	Audio* truck;
	bool song_started;
	bool going_back;
	void OnYellowClick();
	void OnBlueClick();
	void MusicOnClick();
	void OnKeyPressed(Key key);
};

