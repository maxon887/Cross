/*	Copyright © 2018 Maksim Lukyanov

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

namespace FMOD {
	class Sound;
	class Channel;
}

namespace cross{

/*	By this class you can play short sound from memory or stream music from hard drive.
	Underneath Audio os will try to use default os playback device */
class Sound {
public:
	Sound(const String& file, bool loop, bool isStream);
	~Sound();

	/* Will play audio file which was provided as a constructor parameter */
	void Play();
	/* Will pause audio without releasing resources */
	void Pause();
	/* Will continue playing Sound from moment where it was paused */
	void Resume();
	/* Stop playing sound. Audio resources may be released */
	void Stop();
	/* Returns true if audio currently playing */
	bool IsPlaying() const;
	/* Weak clone. If original object will be deleted. Cloned copy will stop working */
	Sound* Clone() const;

private:
	FMOD::Sound* sound		= nullptr;
	FMOD::Channel* channel	= nullptr;
	bool original			= true;

	Sound(const Sound& obj);
};

};
