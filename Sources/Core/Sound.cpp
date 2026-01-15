#include "Sound.h"
#include "Internals/Audio.h"
#include "System.h"

#include "Libs/FMOD/fmod.hpp"
#include "Libs/FMOD/fmod_errors.h"

using namespace cross;

static FMOD_RESULT result;

Sound::Sound(const String& path, bool loop, bool stream) {
	sound = audio->LoadSound(path, loop, stream);
}

Sound::Sound(const Sound& obj) :
	sound(obj.sound),
	channel(obj.channel),
	original(false)
{ }

Sound::~Sound() {
	if(original){
		result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	}else{
		result = channel->stop();
	}
	CROSS_ASSERT(result == FMOD_OK, "Error while destroying Sound");
}

void Sound::Play() {
	result = audio->GetSystem()->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
	audio->GetSystem()->update();
}

void Sound::Pause() {
	result = channel->setPaused(true);
	ERRCHECK(result);
}

void Sound::Resume() {
	result = channel->setPaused(false);
	ERRCHECK(result);
}

void Sound::Stop() {
	channel->stop();
}

bool Sound::IsPlaying() const {
	bool playing;
	result = channel->isPlaying(&playing);
	if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
		ERRCHECK(result);
	}
	return playing;
}

Sound* Sound::Clone() const {
	return CREATE Sound(*this);
}