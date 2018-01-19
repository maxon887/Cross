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
#include "Sound.h"
#include "Internals/Audio.h"
#include "System.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

using namespace cross;

static FMOD_RESULT result;

Sound::Sound(const string& path, bool loop, bool stream) {
	sound = audio->LoadSound(path, loop, stream);
}

Sound::Sound(const Sound& obj) :
	sound(obj.sound),
	channel(obj.channel),
	original(false)
{ }

Sound::~Sound() {
	if(original){
		result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	}else{
		result = channel->stop();
	}
	CROSS_ASSERT(result == FMOD_OK, "Error while destroing Sound");
}

void Sound::Play() {
	result = audio->GetSystem()->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
	audio->GetSystem()->update();
}

void Sound::Pause() {
	result = channel->setPaused(true);
	ERRCHECK(result);
}

void Sound::Resume() {
	result = channel->setPaused(false);
	ERRCHECK(result);
}

void Sound::Stop() {
	channel->stop();
}

bool Sound::IsPlaying() const {
	bool playing;
	result = channel->isPlaying(&playing);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
		ERRCHECK(result);
	}
	return playing;
}

Sound* Sound::Clone() const {
	return new Sound(*this);
}