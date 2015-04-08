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
	
#include "Animation.h"

Animation::Animation(float rate, Image* frames, int frameCount){
	this->duration = 0xFF;
	this->frame_num = 0;
	this->looped = false;
	this->rate = rate;
	this->frames = frames;
	this->frame_count = frameCount;
}

Animation::Animation(float rate, Image* frames, int frameCount, bool looped){
	this->duration = 0xFF;
	this->frame_num = 0;
	this->looped = looped;
	this->rate = rate;
	this->frames = frames;
	this->frame_count = frameCount;
}

void Animation::Start(){
	duration = 0;
}

void Animation::Update(float sec){
	if(duration > frame_count * rate && !looped){
		frame_num = 0;
		duration = 0xFF;
	}else{
		frame_num = (int)(duration / rate);
		frame_num = frame_num % frame_count;
		duration += sec;
	}
}

Image* Animation::GetImage(){
	return &frames[frame_num];
}

bool Animation::IsRunnig(){
	return duration != 0xFF;
}