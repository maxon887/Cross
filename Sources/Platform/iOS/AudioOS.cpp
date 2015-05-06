/*	Copyright � 2015 Lukyanau Maksim
 
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


#include "AudioOS.h"

FMOD::System*	AudioOS::system;
FMOD_RESULT		AudioOS::result;
unsigned int	AudioOS::version;
void*			AudioOS::extradriverdata;

void AudioOS::Init(){
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
}

void AudioOS::Release(){
    result = system->close();
    ERRCHECK(result);
}

AudioOS::AudioOS(string path, bool loop, bool isStream){
    FMOD_MODE mode = 0;
    channel = NULL;
    if(loop)
        mode = FMOD_LOOP_NORMAL;
    else
        mode = FMOD_LOOP_OFF;
    if(isStream)
        mode |= FMOD_CREATESTREAM;
    
    result = system->createSound(Common_MediaPath(path.c_str()), mode, 0, &sound);
    ERRCHECK(result);
}


void AudioOS::Play(){
    result = system->playSound(sound, 0, false, &channel);
    ERRCHECK(result);
}

void AudioOS::Pause(){
    result = channel->setPaused(true);
    ERRCHECK(result);
}

void AudioOS::Resume(){
    result = channel->setPaused(false);
    ERRCHECK(result);
}

void AudioOS::Stop(){
    channel->stop();
}

bool AudioOS::IsPlaying(){
    bool playing;
    result = channel->isPlaying(&playing);
    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
        ERRCHECK(result);
    }
    return playing;
}
