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

int FMOD_Main()
{
    FMOD::System     *system;
    FMOD::Sound      *sound, *sound_to_play;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    unsigned int      version;
    void             *extradriverdata = 0;
    int               numsubsounds;

    Common_Init(&extradriverdata);

    /*
        Create a System object and initialize.
    */
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



    /*
        This example uses an FSB file, which is a preferred pack format for fmod containing multiple sounds.
        This could just as easily be exchanged with a wav/mp3/ogg file for example, but in this case you wouldnt need to call getSubSound.
        Because getNumSubSounds is called here the example would work with both types of sound file (packed vs single).
    */
    //result = system->createSound(Common_MediaPath("wave_vorbis.fsb"), FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
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

    /*
        Play the sound.
    */
    result = system->playSound(sound_to_play, 0, false, &channel);
    ERRCHECK(result);

    /*
        Main loop.
    */
    Common_Sleep(300000);

    do
    {
        Common_Update();


        result = system->update();
        ERRCHECK(result);


            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool         playing = false;
            bool         paused = false;


        Common_Sleep(50);
    } while (true);

    /*
        Shut down
    */

    result = sound->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    Common_Close();
    return 0;
}
