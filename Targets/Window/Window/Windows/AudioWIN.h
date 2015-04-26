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
#include "LauncherWIN.h"
#include "Sound.h"
#include "Music.h"
#include "portaudio.h"
#include "sndfile.h"

#define SAMPLE_RATE (44100)

class AudioWIN : public Sound, public Music{
//User module
public:
	AudioWIN(Launcher* launcher, string filename, bool loop);
	void Play();
	void Stop();
	void Pause();
	void CheckPaError(PaError err);
	~AudioWIN();
	SNDFILE* file;
	PaStream* stream;
	bool loop;
private:
	Launcher* launcher;
	string source;
	//char source[BUF_LEN];
	//char str_buf[BUF_LEN];
//Framework module. You don't need call any of this methods or modify variable
};