/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/

#pragma once

#include "Game.h"
#include "Sound.h"
#include "Music.h"
#include "fmod.hpp"
#include "common.h"

class AudioAndroid : public Sound,
					 public Music{
public:
	static void Init();
	static void Release();
	AudioAndroid(string path, bool loop, bool isStream);
	void Play();
	void Pause();
	void Resume();
	void Stop();
	bool IsPlaying();
private:
	static FMOD::System* system;
	static FMOD_RESULT result;
	static unsigned int version;
	static void* extradriverdata;

	static mutex audio_mutex;
	static AudioAndroid* current_audio;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
};
