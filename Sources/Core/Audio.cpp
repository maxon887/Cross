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
#include "System.h"
#include "Sound.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

#define ERRCHECK(_result) if(_result != FMOD_OK) { throw CrossException("FMOD error %d - %s", _result, FMOD_ErrorString(_result)); }

using namespace cross;

static FMOD_RESULT result;

Audio::Audio() {
	sys->LogIt("Audio::Audio()");
	result = FMOD::System_Create(&fmod_system);
	ERRCHECK(result);

	U32 version;
	result = fmod_system->getVersion(&version);
	ERRCHECK(result);

	if(version < FMOD_VERSION){
		throw CrossException("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = fmod_system->init(32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);
}

Audio::~Audio(){
	result = fmod_system->close();
	if(result != FMOD_OK){
		sys->LogIt("Error while closing FMOD system");
	}
}

Sound* Audio::LoadSound(const string& path, bool loop, bool stream) {
	if(sys == NULL){
		throw CrossException("Audio not initialized");
	}
	Sound* sound = new Sound();

	FMOD_MODE mode = 0;
	if(loop){
		mode = FMOD_LOOP_NORMAL;
	}else{
		mode = FMOD_LOOP_OFF;
	}
	if(stream){
		mode |= FMOD_CREATESTREAM;
	}
#ifdef ANDROID
	string absPath = "file:///android_asset/" + path;
#else
	string absPath = sys->AssetsPath() + "/" + path;
#endif
	result = fmod_system->createSound(absPath.c_str(), mode, 0, &sound->sound);
    ERRCHECK(result);

	return sound;
}

void Audio::Suspend(){
	fmod_system->mixerSuspend();
}

void Audio::Resume(){
	fmod_system->mixerResume();
}