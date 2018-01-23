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
#pragma once
#include "Cross.h"

#define ERRCHECK(_result) \
if(_result != FMOD_OK) { \
	CROSS_ASSERT(false, "FMOD error %d - %s", _result, FMOD_ErrorString(_result)); \
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
	FMOD::Sound* LoadSound(const string& path, bool loop, bool isStream);
	void Suspend();
	void Resume();

private:
	FMOD::System* fmod_system = null;
};

}
