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

#include "Graphics.h"
#include <vector>

namespace cross {

class Animation{
public:
	Animation(Animation& anim);
	Animation(Graphics* gfx, float rate, Image* frames[], int frameCount);
	Animation(Graphics* gfx, float rate, Image* frames[], int frameCount, bool looped);
	void Start();
	void Update(float sec);
	Image* GetImage();
	bool IsRunning();
	~Animation();
private:
	void Init(Graphics* gfx, float rate, Image* frames[], int frameCount, bool looped);
	Graphics* graphics;
	vector<Image*> frames;
	float rate;
	float duration;
	int frame_num;
	bool looped;
	bool original;
};

}