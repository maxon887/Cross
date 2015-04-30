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
#include <android/log.h>

FMOD::System*	AudioAndroid::system;
FMOD_RESULT		AudioAndroid::result;
unsigned int	AudioAndroid::version;
void*			AudioAndroid::extradriverdata;

mutex 			AudioAndroid::audio_mutex;
AudioAndroid*	AudioAndroid::current_audio;

void AudioAndroid::Init(){

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
    ERRCHECK(result);

    FMOD::Sound* sound;
    FMOD::Channel* channel;

    __android_log_print(ANDROID_LOG_DEBUG, "Cross++", "Is about to play");
    audio_mutex.lock();
    audio_mutex.lock();
	result = system->createSound(Common_MediaPath("Eskimo.mp3"), FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	ERRCHECK(result);


	result = system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
    __android_log_print(ANDROID_LOG_DEBUG, "Cross++", "Is playing");
/*
    FMOD::Sound* sound;
    FMOD::Channel* channel;

	result = system->createSound(Common_MediaPath("Eskimo.mp3"), FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	ERRCHECK(result);


	result = system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);*/

}

AudioAndroid::AudioAndroid(string path, bool loop, bool isStream){/*
	FMOD_MODE mode = 0;
	if(loop)
		mode = FMOD_LOOP_NORMAL;
	else
		mode = FMOD_LOOP_OFF;
	if(isStream)
		mode |= FMOD_CREATESTREAM;

	result = system->createSound(path.c_str(), mode, 0, &sound);
    ERRCHECK(result);*/
}

void AudioAndroid::Play(){
	//result = system->playSound(sound, 0, false, &channel);
	//ERRCHECK(result);
	//current_audio = this;
	//audio_mutex.unlock();
	audio_mutex.unlock();
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
