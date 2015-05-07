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

FMOD::System*	Audio::system;
FMOD_RESULT		Audio::result;
unsigned int	Audio::version;
void*			Audio::extradriverdata;
Launcher*		Audio::launcher;

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)
void (*Common_Private_Error)(FMOD_RESULT, const char *, int);

void Common_Fatal(const char *format, ...)
{
    char error[1024];

    va_list args;
    va_start(args, format);
    Common_vsnprintf(error, 1024, format, args);
    va_end(args);
    error[1023] = '\0';

}

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        if (Common_Private_Error)
        {
            Common_Private_Error(result, file, line);
        }
        Common_Fatal("%s(%d): FMOD error %d - %s", file, line, result, FMOD_ErrorString(result));
    }
}


void Audio::Init(Launcher* launcher){
	Audio::launcher = launcher;
	//Common_Init(&extradriverdata);
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

void Audio::Release(){
    result = system->close();
    ERRCHECK(result);
}

Audio::Audio(string path, bool loop, bool isStream){
	FMOD_MODE mode = 0;
	channel = NULL;
	if(loop)
		mode = FMOD_LOOP_NORMAL;
	else
		mode = FMOD_LOOP_OFF;
	if(isStream)
		mode |= FMOD_CREATESTREAM;
	if(launcher != NULL)
		path = launcher->AssetsPath() + path;
	else
		path = "file:///android_asset/" + path;

	result = system->createSound(path.c_str(), mode, 0, &sound);
    ERRCHECK(result);
}

void Audio::Play(){
	result = system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
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

Audio::~Audio(){
	result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
    ERRCHECK(result);
}
