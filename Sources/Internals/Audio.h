#pragma once
#include "Cross.h"

#define ERRCHECK(_result) \
if(_result != FMOD_OK) { \
	CROSS_ASSERT(false, "FMOD error # - #", _result, FMOD_ErrorString(_result)); \
}

namespace FMOD{
	class Sound;
	class System;
}

namespace cross {

/*	Class responsible for loading sound and music */
class Audio {
public:
	Audio();
	~Audio();

	FMOD::System* GetSystem();
	FMOD::Sound* LoadSound(const String& path, bool loop, bool isStream);
	void Suspend();
	void Resume();

private:
	FMOD::System* fmod_system = nullptr;
};

}
