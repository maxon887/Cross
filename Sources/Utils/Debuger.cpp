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
	
#include "Debuger.h"

Debuger::Debuger(Game* game){
	input = game->input;
	texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 1.0f);
	update_time = 0;
	update_sum = 0;
	update_counter = 0;
	render_time = 0;
	render_sum = 0;
	render_counter = 0;
	time = 0;
}

void Debuger::Display(float micro){
	time += micro / 1000000.0f;
	if(render_counter == 20){
		render_counter = 0;
		render_time = render_sum / 20.0f / 1000.0f;
		render_sum = 0;
	} else {
		render_sum += micro;
		render_counter++;
	}
	sprintf(buffer, "Render Time: %fms", render_time);
	texter->DrawText(0, 0, buffer);

	if(update_time == 0){
		texter->DrawText(0, texter->GetHeight(), "Update Time: Undefined");
	} else {
		sprintf(buffer, "Update Time: %fms", update_time);
		texter->DrawText(0, texter->GetHeight(), buffer);
	}

	if(render_time == 0){
		texter->DrawText(0, texter->GetHeight() * 2, "FPS: Infinitive");
	} else {
		sprintf(buffer, "FPS: %f", 1000.0f/render_time);
		texter->DrawText(0, texter->GetHeight() * 2, buffer);
	}

	if(input->HaveInput()){
		PointX in = input->GetInput();
		sprintf(buffer, "Input: x=%f, y=%f", in.x, in.y);
		texter->DrawText(0, texter->GetHeight() * 3, buffer);
	}else{
		texter->DrawText(0, texter->GetHeight() * 3, "Input: UP");
	}

	sprintf(buffer, "Run time: %f", time);
	texter->DrawText(0, texter->GetHeight() * 4, buffer);
}

void Debuger::SetUpdateTime(float micro) {
	if(update_counter == 20){
		update_counter = 0;
		update_time = update_sum / 20.0f / 1000.0f;
		update_sum = 0;
	} else {
		update_sum += micro;
		update_counter++;
	}
}
