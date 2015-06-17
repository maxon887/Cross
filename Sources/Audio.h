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
#include "Launcher.h"

#include "fmod.hpp"

namespace cross {

class Audio{
public:
	static void Init(Launcher* launcher);
	static void Suspend();
	static void Release();
	Audio(string path, bool loop, bool isStream);
	void Play();
	void Pause();
	void Resume();
	void Stop();
	bool IsPlaying();
	~Audio();
private:
	static FMOD::System* system;
	static FMOD_RESULT result;
	static unsigned int version;
	static void* extradriverdata;
	static Launcher* launcher;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
};

}
