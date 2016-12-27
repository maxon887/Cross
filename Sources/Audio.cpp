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
#include "Sound.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

#define ERRCHECK(_result) if(_result != FMOD_OK) { throw CrossException("FMOD error %d - %s", _result, FMOD_ErrorString(_result)); }

using namespace cross;

static FMOD_RESULT result;

Audio::Audio() : 
	system(NULL)	
{
	launcher->LogIt("Audio::Audio()");
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

Audio::~Audio(){
	result = system->close();
    ERRCHECK(result);
}

Sound* Audio::LoadSound(const string& path, bool loop, bool stream) {
	if(system == NULL){
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
	string absPath = launcher->AssetsPath() + "/" + path;
#endif
	result = system->createSound(absPath.c_str(), mode, 0, &sound->sound);
    ERRCHECK(result);

	return sound;
}

void Audio::Suspend(){
	system->mixerSuspend();
}

void Audio::Resume(){
	system->mixerResume();
}