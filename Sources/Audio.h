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

#include <string>

namespace FMOD{
	class System;
	class Sound;
	class Channel;
}

namespace cross {

class Launcher;

/* Class responsible for playing audio.
	Additional dependencies needed to be included for using this class. 
	If you don not use audio in your game declare DISABLE_AUDIO directive */
class Audio{
public:
	Audio(std::string path, bool loop, bool isStream);
	Audio(Audio& obj);

	void Play();
	void Pause();
	void Resume();
	void Stop();
	bool IsPlaying();
	~Audio();
//Internal data. You don't need call any of this methods or modify variables
public:
	static void Init(Launcher* launcher);
	static void SuspendSystem();
	static void ResumeSystem();
	static void Release();
private:
	static FMOD::System* system;
	static unsigned int version;
	static void* extradriverdata;
	static Launcher* launcher;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
	bool original;
};

}
