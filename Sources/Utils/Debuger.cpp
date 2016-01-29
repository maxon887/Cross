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
#include "Launcher.h"
#include "Texter.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Game.h"

#include <chrono>

using namespace cross;
using namespace chrono;

void Debuger::StartCheckTime(){
	CrossTime check_time = high_resolution_clock::now();
	times.push_back(check_time);
}

void Debuger::StopCheckTime(string label){
	CrossTime now = high_resolution_clock::now();
	CrossTime check_time = times.back();
	times.pop_back();
	auto up = duration_cast<microseconds>(now - check_time).count();
	double milis = up/1000.0;
	string msg = label + to_string(milis) + "ms";
	if(launcher != NULL)
		launcher->LogIt(msg);
}

Debuger::Debuger(){
	touch_pointer = NULL;
	texter = NULL;
	update_time = 0;
	update_sum = 0;
	update_counter = 0;
	render_time = 0;
	render_sum = 0;
	render_counter = 0;
	time = 0;
	screen_debug = false;
	console_debug = false;
	touches = false;
	touch_down = false;
	next_display = 3000000.f;
	input->ActionDown += MakeDelegate(this, &Debuger::OnActionDown);
	input->ActionMove += MakeDelegate(this, &Debuger::OnActionMove);
	input->ActionUp += MakeDelegate(this, &Debuger::OnActionUp);
}

Debuger::~Debuger(){
	delete texter;
}

void Debuger::Display(float micro){
	if(screen_debug || console_debug){
		time += micro / 1000000.f;
		if(render_counter == 20){
			render_counter = 0;
			render_time = render_sum / 20.f / 1000.f;
			render_sum = 0;
		}else{
			render_sum += micro;
			render_counter++;
		}
	}
	if(screen_debug){
		float height = game->GetHeight();
		texter->DrawText(0, height - texter->GetHeight(), "Render Time: " + to_string(render_time) + "ms");
		if(update_time == 0){
			texter->DrawText(0, height - texter->GetHeight() * 2, "Update Time: Undefined");
		}else{
			texter->DrawText(0, height - texter->GetHeight() * 2, "Update Time: " + to_string(update_time) + "ms");
		}
		if(render_time == 0){
			texter->DrawText(0, height - texter->GetHeight() * 3, "FPS: Infinitive");
		}else{
			texter->DrawText(0, height - texter->GetHeight() * 3, "FPS: " + to_string(1000.f / render_time));
		}
		if(touch_down){
			texter->DrawText(0, height - texter->GetHeight() * 4, "Input x: " + to_string(touch_pos.x) + " y: " + to_string(touch_pos.y));
		}else{
			texter->DrawText(0, height - texter->GetHeight() * 4, "Input Up");
		}
		texter->DrawText(0, height - texter->GetHeight() * 5, "Run time: " + to_string(time));
	}
	if(console_debug){
		if(next_display < 0){
			string msg = "";
			msg += "Render Time: " + to_string(render_time) + "ms\n";
			msg += "Update Time: " + to_string(update_time) + "ms\n";
			msg += "FPS: " + to_string(1000.f/render_time) + "\n";
			msg += "=================================";
			launcher->LogIt(msg);
			next_display = 3000000.f;
		}else{
			next_display -= micro;
		}
	}
}

void Debuger::EnableScreenDebug(){
	if(texter == NULL){
		texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 1.0f);
	}else{
		launcher->LogIt("Warning!Screen debug already anabled");
	}
	screen_debug = true;
}

void Debuger::EnableConsoleDebug(){
	console_debug = true;
}

void Debuger::EnableTouches(){
	if(touch_pointer == NULL){
		touch_pointer = gfx2D->LoadImage("TouchPointer.png", game->GetScaleFactor() * 0.5f);
	} else{
		launcher->LogIt("Warning!Touches already anabled");
	}
	touches = true;
}

void Debuger::SetUpdateTime(float micro) {
	if(screen_debug || console_debug){
		if(update_counter == 20){
			update_counter = 0;
			update_time = update_sum / 20.0f / 1000.0f;
			update_sum = 0;
		}else{
			update_sum += micro;
			update_counter++;
		}
	}
}

float Debuger::GetFPS(){
	return 1000.f / render_time;
}

void Debuger::OnActionDown(Vector2D pos){
	touch_down = true;
	touch_pos = pos;
}

void Debuger::OnActionUp(Vector2D pos){
	touch_down = false;
}

void Debuger::OnActionMove(Vector2D pos){
	touch_pos = pos;
}


Texter* Debuger::GetTexter(){
	return texter;
}
