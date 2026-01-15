#pragma once
#include "Cross.h"

namespace FMOD {
	class Sound;
	class Channel;
}

namespace cross{

/*	By this class you can play short sound from memory or stream music from hard drive.
	Underneath Audio os will try to use default os playback device */
class Sound {
public:
	Sound(const String& file, bool loop, bool isStream);
	~Sound();

	/* Will play audio file which was provided as a constructor parameter */
	void Play();
	/* Will pause audio without releasing resources */
	void Pause();
	/* Will continue playing Sound from moment where it was paused */
	void Resume();
	/* Stop playing sound. Audio resources may be released */
	void Stop();
	/* Returns true if audio currently playing */
	bool IsPlaying() const;
	/* Weak clone. If original object will be deleted. Cloned copy will stop working */
	Sound* Clone() const;

private:
	FMOD::Sound* sound		= nullptr;
	FMOD::Channel* channel	= nullptr;
	bool original			= true;

	Sound(const Sound& obj);
};

}
