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
#include "Audio.h"
#include "Launcher.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

using namespace cross;

#ifndef DISABLE_AUDIO

static FMOD_RESULT		result;

FMOD::System*	Audio::system = NULL;

#define ERRCHECK(result) if(result != FMOD_OK) { throw CrossException("FMOD error %d - %s", result, FMOD_ErrorString(result)); }

void Audio::Init(){
	launcher->LogIt("Audio::Init()");
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	U32 version;
	result = system->getVersion(&version);
	ERRCHECK(result);

	if(version < FMOD_VERSION){
		throw CrossException("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = system->init(32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
}

void Audio::SuspendSystem(){
	system->mixerSuspend();
}

void Audio::ResumeSystem(){
	system->mixerResume();
}

void Audio::Release(){
    result = system->close();
    ERRCHECK(result);
}

Audio::Audio(const string& path, bool loop, bool isStream){
	if(system == NULL)
		throw CrossException("Audio not initialized");
	FMOD_MODE mode = 0;
	channel = NULL;
	original = true;
	if(loop){
		mode = FMOD_LOOP_NORMAL;
	}else{
		mode = FMOD_LOOP_OFF;
	}
	if(isStream){
		mode |= FMOD_CREATESTREAM;
	}
#ifdef ANDROID
	string absPath = "file:///android_asset/" + path;
#else
	string absPath = launcher->AssetsPath() + "/" + path;
#endif
	result = system->createSound(absPath.c_str(), mode, 0, &sound);
    ERRCHECK(result);
}

Audio::Audio(Audio& obj){
	original = false;
	channel = obj.channel;
	sound = obj.sound;
}

void Audio::Play(){
	result = system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
	system->update();
}

void Audio::Pause(){
	result = channel->setPaused(true);
	ERRCHECK(result);
}

void Audio::Resume(){
	result = channel->setPaused(false);
	ERRCHECK(result);
}

void Audio::Stop(){
	channel->stop();
}

bool Audio::IsPlaying(){
	bool playing;
	result = channel->isPlaying(&playing);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
		ERRCHECK(result);
	}
	return playing;
}

Audio* Audio::Clone(){
	return new Audio(*this);
}

Audio::~Audio(){
	if(original){
		result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
		ERRCHECK(result);
	}else{
		channel->stop();
	}
}

#else


void Audio::Init(){
	launcher->LogIt("Audio::Init()");
	launcher->LogIt("Audio system disabled");
}

void Audio::SuspendSystem(){
}

void Audio::ResumeSystem(){
}

void Audio::Release(){
}

Audio::Audio(string path, bool loop, bool isStream){
	throw CrossException("Audio system disabled");
}

Audio::Audio(Audio& obj){
	throw CrossException("Audio system disabled");
}

void Audio::Play(){
	throw CrossException("Audio system disabled");
}

void Audio::Pause(){
	throw CrossException("Audio system disabled");
}

void Audio::Resume(){
	throw CrossException("Audio system disabled");
}

void Audio::Stop(){
	throw CrossException("Audio system disabled");
}

bool Audio::IsPlaying(){
	throw CrossException("Audio system disabled");
}

Audio::~Audio(){
	throw CrossException("Audio system disabled");
}

#endif