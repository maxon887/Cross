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

using namespace cross;

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
	console_debug(false),
	touches(false),
	touch_down(false),
	next_display(3000000.f),
	debugger_font(nullptr)
{ 
	memset(params, 0, sizeof(params));
}

Debugger::~Debugger(){
	delete debugger_font;
}

void Debugger::StartCheckTime() {
	unsigned long checkTime = launcher->GetTime();
	times.push_back(checkTime);
}

void Debugger::StopCheckTime(string label) {
	unsigned long now = launcher->GetTime();
	unsigned long checkTime = times.back();
	times.pop_back();
	double milis = (now - checkTime) / 1000.0;
	string msg = label + to_string(milis) + "ms";
	if(launcher != NULL)
		launcher->LogIt(msg);
}

void Debugger::Display(float micro){
	if(render_counter == 20){
		render_counter = 0;
		render_time = render_sum / 20.f / 1000.f;
		render_sum = 0;
	}else{
		render_sum += micro;
		render_counter++;
	}

	int optionPosition = 1;
	float height = game->GetCurrentScreen()->GetHeight();
	char outputString[256];
	if(params[Parameter::RENDER_TIME] == true){
		sprintf(outputString, "Render Time: %0.1fms", render_time);
		gfx2D->DrawText(Vector2D(0.f, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		optionPosition++;
	}
	if(params[Parameter::UPDATE_TIME] == true){
		if(update_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "Update Time: -", debugger_font);
		}else{
			sprintf(outputString, "Update Time: %0.1fms", update_time);
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		}
		optionPosition++;
	}
	if(params[Parameter::FPS] == true){
		if(render_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "FPS: -", debugger_font);
		}else{
			sprintf(outputString, "FPS: %0.1f", 1000.f / render_time);
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		}
		optionPosition++;
	}
	if(params[Parameter::RUN_TIME] == true){
		sprintf(outputString, "Run time: %0.2fsec", game->GetRunTime());
		gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		optionPosition++;
	}
	if(params[Parameter::INPUT] == true){
		if(touch_down) {
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "Input x: " + to_string(touch_pos.x) + " y: " + to_string(touch_pos.y), debugger_font);
		} else {
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "Input Up", debugger_font);
		}
		optionPosition++;
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

void Debugger::ConsoleDebug(bool enable){
	console_debug = enable;
}

void Debugger::SetUpdateTime(float micro) {
	if(update_counter == 20){
		update_counter = 0;
		update_time = update_sum / 20.0f / 1000.0f;
		update_sum = 0;
	}else{
		update_sum += micro;
		update_counter++;
	}
}

void Debugger::EnableDebug(Parameter param){
	params[param] = true;
	if(!debugger_font){
		debugger_font = gfx2D->GetDefaultFont()->Clone();
		if(launcher->GetTargetWidth() < 700){
			debugger_font->SetSize(37.f);
		}else{
			debugger_font->SetSize(25.f);
		}
	}
	if(param == Parameter::INPUT){
		action_down_delegate = MakeDelegate(this, &Debugger::OnActionDown);
		action_move_delegate = MakeDelegate(this, &Debugger::OnActionMove);
		action_up_delegate = MakeDelegate(this, &Debugger::OnActionUp);
		input->ActionDown += action_down_delegate;
		input->ActionMove += action_move_delegate;
		input->ActionUp += action_up_delegate;
	}
}

void Debugger::DisableDebug(Parameter param){
	params[param] = false;
	if(param == Parameter::INPUT){
		input->ActionDown -= action_down_delegate;
		input->ActionMove -= action_move_delegate;
		input->ActionUp -= action_up_delegate;
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