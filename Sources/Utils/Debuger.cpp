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

#include <vector>

using namespace cross;
using namespace chrono;

static Launcher* launcher = NULL;
static vector<time_point<high_resolution_clock>> times;

void Debuger::StartCheckTime(){
	time_point<high_resolution_clock> check_time = high_resolution_clock::now();
	times.push_back(check_time);
}

void Debuger::StopCheckTime(string label){
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	time_point<high_resolution_clock> check_time = times.back();
	times.pop_back();
	auto up = duration_cast<microseconds>(now - check_time).count();
	double milis = up/1000.0;
	string msg = label + to_string(milis) + "ms";
	if(launcher != NULL)
		launcher->LogIt(msg);
}

Debuger::Debuger(Game* game){
	this->game = game;
	launcher = game->launcher;
	input = game->input;
	texter = NULL;
	touch_pointer = NULL;
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
	next_display = 3000000.f;
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
		game->graphics->SetViewPort(Point(0, 0), 900, 900 / launcher->DeviceAspect());
		texter->DrawText(0, 0, "Render Time: " + to_string(render_time) + "ms");
		if(update_time == 0){
			texter->DrawText(0, texter->GetHeight(), "Update Time: Undefined");
		}else{
			texter->DrawText(0, texter->GetHeight(), "Update Time: " + to_string(update_time) + "ms");
		}
		if(render_time == 0){
			texter->DrawText(0, texter->GetHeight() * 2, "FPS: Infinitive");
		}else{
			texter->DrawText(0, texter->GetHeight() * 2, "FPS: " + to_string(1000.f/render_time));
		}
		if(input->HaveInput()){
			Point in = input->GetInput();
			texter->DrawText(0, texter->GetHeight() * 3, "Input Virtual: x=" + to_string(in.x) + " y=" + to_string(in.y));
			texter->DrawText(0, texter->GetHeight() * 4, "Input Real: x=" + to_string(input->input_loc.x) + " y=" + to_string(input->input_loc.y));
		}else{
			texter->DrawText(0, texter->GetHeight() * 3, "Input Virtual: UP");
			texter->DrawText(0, texter->GetHeight() * 4, "Input Real: UP");
		}
		texter->DrawText(0, texter->GetHeight() * 5, "Run time: " + to_string(time));
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
	if(touches){
		if(input->HaveInput()){
			game->graphics->DrawImage(input->GetInput(), touch_pointer);
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
		touch_pointer = game->graphics->LoadImage("TouchPointer.png", game->GetScaleFactor() * 0.5f);
	}else{
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
		} else {
			update_sum += micro;
			update_counter++;
		}
	}
}











