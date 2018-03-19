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
#include "Audio.h"
#include "System.h"
#include "Sound.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

using namespace cross;

static FMOD_RESULT result;

Audio::Audio() {
	system->LogIt("Audio::Audio()");
	result = FMOD::System_Create(&fmod_system);
	ERRCHECK(result);

	U32 version;
	result = fmod_system->getVersion(&version);
	ERRCHECK(result);

	system->LogIt("FMOD Version - %08x", version);

	CROSS_ASSERT(version > FMOD_VERSION, "FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

	result = fmod_system->init(32, FMOD_INIT_NORMAL, nullptr);
	ERRCHECK(result);
}

Audio::~Audio() {
	result = fmod_system->close();
	CROSS_ASSERT(result != FMOD_OK, "Error while closing FMOD system");
}

FMOD::System* Audio::GetSystem() {
	return fmod_system;
}

FMOD::Sound* Audio::LoadSound(const String& path, bool loop, bool stream) {
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
	String absPath = "file:///android_asset/" + path;
#else
	String absPath = system->AssetsPath() + "/" + path;
#endif
	FMOD::Sound* sound = nullptr;
	result = fmod_system->createSound(absPath.c_str(), mode, 0, &sound);
	ERRCHECK(result);

	return sound;
}

void Audio::Suspend(){
	fmod_system->mixerSuspend();
}

void Audio::Resume(){
	fmod_system->mixerResume();
}
