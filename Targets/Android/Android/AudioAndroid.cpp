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


#include "AudioAndroid.h"

FMOD::System*	AudioAndroid::system;
FMOD_RESULT		AudioAndroid::result;
unsigned int	AudioAndroid::version;
void*			AudioAndroid::extradriverdata;

void AudioAndroid::Init(){/*
	Common_Init(&extradriverdata);
	result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

	if (version < FMOD_VERSION)
    {
        Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);*/
}

AudioAndroid::AudioAndroid(unsigned char* data, int length, bool loop){

}

void AudioAndroid::Play(){

}

void AudioAndroid::Pause(){

}

void AudioAndroid::Stop(){

}

void AudioAndroid::Resume(){

}

bool AudioAndroid::IsPlaying(){
	return false;
}
