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
#pragma once
#include "Cross.h"

namespace cross {

class Animation{
public:
	Animation(Animation& anim);
	Animation(float rate, Sprite* frames[], U32 frameCount);
	Animation(float rate, Sprite* frames[], U32 frameCount, bool looped);
	~Animation();

	void Start();
	void Update(float sec);
	Sprite* GetImage();
	bool IsRunning();

private:
	Array<Sprite*> frames;
	float rate;
	float duration;
	U32 frame_num;
	bool looped;
	bool original;
};

}