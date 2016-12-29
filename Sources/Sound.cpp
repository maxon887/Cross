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
#include "Sound.h"
#include "Audio.h"
#include "System.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

#define ERRCHECK(_result) if(_result != FMOD_OK) { throw CrossException("FMOD error %d - %s", _result, FMOD_ErrorString(_result)); }

using namespace cross;

static FMOD_RESULT result;

Sound::Sound() :
	sound(NULL),
	channel(NULL),
	original(true)
{ }

Sound::Sound(Sound& obj) :
	sound(obj.sound),
	channel(obj.channel),
	original(false)
{ }

Sound::~Sound(){
	if(original){
		result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	}else{
		result = channel->stop();
	}
	if(result != FMOD_OK){
		system->LogIt("Error while destroing Sound");
	}
}

void Sound::Play(){
	result = audio->fmod_system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
	audio->fmod_system->update();
}

void Sound::Pause(){
	result = channel->setPaused(true);
	ERRCHECK(result);
}

void Sound::Resume(){
	result = channel->setPaused(false);
	ERRCHECK(result);
}

void Sound::Stop(){
	channel->stop();
}

bool Sound::IsPlaying(){
	bool playing;
	result = channel->isPlaying(&playing);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
		ERRCHECK(result);
	}
	return playing;
}

Sound* Sound::Clone(){
	return new Sound(*this);
}