/*==============================================================================
Play Stream Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2015.

This example shows how to simply play a stream such as an MP3 or WAV. The stream
behaviour is achieved by specifying FMOD_CREATESTREAM in the call to
System::createSound. This makes FMOD decode the file in realtime as it plays,
instead of loading it all at once which uses far less memory in exchange for a
small runtime CPU hit.
==============================================================================*/
#include "fmod.hpp"
#include "common.h"
#include "thread"
#include "AudioAndroid.h"


int FMOD_Main()
{
AudioAndroid::Init();
    //}
    /*
        This example uses an FSB file, which is a preferred pack format for fmod containing multiple sounds.
        This could just as easily be exchanged with a wav/mp3/ogg file for example, but in this case you wouldnt need to call getSubSound.
        Because getNumSubSounds is called here the example would work with both types of sound file (packed vs single).
    */

    /*
    result = system->createSound(Common_MediaPath("Eskimo.mp3"), FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
    ERRCHECK(result);

    result = sound->getNumSubSounds(&numsubsounds);
    ERRCHECK(result);

    if (numsubsounds)
    {
        sound->getSubSound(0, &sound_to_play);
        ERRCHECK(result);
    }
    else
    {
        sound_to_play = sound;
    }

    result = system->playSound(sound_to_play, 0, false, &channel);
    ERRCHECK(result);*/


/*
    result = sound->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);
    Common_Close();

    */
    return 0;
}
