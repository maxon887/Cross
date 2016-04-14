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
#include "Debugger.h"
#include "Launcher.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Game.h"
#include "Font.h"
#include "Screen.h"

#include <chrono>

using namespace cross;
using namespace chrono;

Debugger* Debugger::instance = nullptr;

Debugger* Debugger::Instance(){
	if(!instance){
		instance = new Debugger();
	}
	return instance;
}

void Debugger::Release(){
	delete instance;
	instance = nullptr;
}

Debugger::Debugger() :
	update_time(0),
	update_sum(0),
	update_counter(0),
	render_time(0),
	render_sum(0),
	render_counter(0),
	time(0),
	screen_debug(false),
	input_debug(false),
	console_debug(false),
	touches(false),
	touch_down(false),
	next_display(3000000.f),
	debugger_font(nullptr)
{ }

Debugger::~Debugger(){
	delete debugger_font;
}

void Debugger::StartCheckTime() {
	CrossTime check_time = high_resolution_clock::now();
	times.push_back(check_time);
}

void Debugger::StopCheckTime(string label) {
	CrossTime now = high_resolution_clock::now();
	CrossTime check_time = times.back();
	times.pop_back();
	auto up = duration_cast<microseconds>(now - check_time).count();
	double milis = up / 1000.0;
	string msg = label + to_string(milis) + "ms";
	if(launcher != NULL)
		launcher->LogIt(msg);
}

void Debugger::Display(float micro){
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
		float height = game->GetCurrentScreen()->GetHeight();
		char outputString[256];
		sprintf(outputString, "Render Time: %0.1fms", render_time);
		//Font* font = gfx2D->GetDefaultFont();
		gfx2D->DrawText(Vector2D(0.f, height - debugger_font->GetSize()), outputString, debugger_font);
		if(update_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 2), "Update Time: -", debugger_font);
		}else{
			sprintf(outputString, "Update Time: %0.1fms", update_time);
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 2), outputString, debugger_font);
		}
		if(render_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 3), "FPS: -", debugger_font);
		}else{
			sprintf(outputString, "FPS: %0.1f", 1000.f / render_time);
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 3), outputString, debugger_font);
		}
		sprintf(outputString, "Run time: %0.2fsec", time);
		gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 4), outputString, debugger_font);
		if(input_debug){
			if(touch_down) {
				gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 5), "Input x: " + to_string(touch_pos.x) + " y: " + to_string(touch_pos.y), debugger_font);
			} else {
				gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * 5), "Input Up", debugger_font);
			}
		}
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

void Debugger::ScreenDebug(bool enable){
	screen_debug = enable;
	if(enable){
		if(!debugger_font){
			debugger_font = gfx2D->GetDefaultFont()->Clone();
			debugger_font->SetSize(debugger_font->GetSize() / 2.f);
		}
	}
}

void Debugger::EnableInputDebug(){
	if(input_debug){
		throw CrossException("Can't enable input debug");
	}
	input_debug = true;
	input->ActionDown += MakeDelegate(this, &Debugger::OnActionDown);
	input->ActionMove += MakeDelegate(this, &Debugger::OnActionMove);
	input->ActionUp += MakeDelegate(this, &Debugger::OnActionUp);
}

void Debugger::ConsoleDebug(bool enable){
	console_debug = enable;
}

void Debugger::SetUpdateTime(float micro) {
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

float Debugger::GetFPS(){
	return 1000.f / render_time;
}

void Debugger::OnActionDown(Input::Action action) {
	touch_down = true;
	touch_pos = action.pos;
}

void Debugger::OnActionUp(Input::Action action) {
	touch_down = false;
}

void Debugger::OnActionMove(Input::Action action) {
	touch_pos = action.pos;
}