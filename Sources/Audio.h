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
#include "Cross.h"

namespace FMOD{
	class System;
	class Sound;
	class Channel;
}

namespace cross {

/*	Audio class provided both stream and cached sound palying. 
	Audio sources must be created themself using constructors of this class. 
	If you dont use audio in your game declare DISABLE_AUDIO */
class Audio{
public:
	Audio(string path, bool loop, bool isStream);
	~Audio();
	
	void Play();
	void Pause();
	void Resume();
	void Stop();
	bool IsPlaying();
	Audio* Clone();
//Internal data. You don't need call any of this methods or modify variables
public:
	static void Init();
	static void SuspendSystem();
	static void ResumeSystem();
	static void Release();

private:
	Audio(Audio& obj);

	static FMOD::System* system;
	static U32 version;
	static void* extradriverdata;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
	bool original;
};

}
