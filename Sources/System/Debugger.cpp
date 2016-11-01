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
	cpu_time(0),
	cpu_sum(0),
	cpu_counter(0),
	update_time(0),
	update_sum(0),
	update_counter(0),
	touches(false),
	touch_down(false),
	debugger_font(nullptr)
{ 
	memset(params, 0, sizeof(params));
}

Debugger::~Debugger(){
	delete debugger_font;
}

void Debugger::SetTimeCheck() {
	U64 checkTime = launcher->GetTime();
	time_checks.push_back(checkTime);
}

float Debugger::GetTimeCheck() {
	U64 now = launcher->GetTime();
	U64 checkTime = time_checks.back();
	time_checks.pop_back();
	return (now - checkTime) / 1000.f;
}

void Debugger::Update(float micro){
	if(update_counter == 20){
		update_counter = 0;
		update_time = update_sum / 20.f / 1000.f;
		update_sum = 0;
	}else{
		update_sum += micro;
		update_counter++;
	}

	S32 optionPosition = 1;
	float height = game->GetCurrentScreen()->GetHeight();
	char outputString[256];
	if(params[Parameter::FPS] == true){
		if(update_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "FPS: -", debugger_font);
		}else{
			sprintf(outputString, "FPS: %0.1f", 1000.f / update_time);
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		}
		optionPosition++;
	}
	if(params[Parameter::UPDATE_TIME] == true){
		sprintf(outputString, "Update Time: %0.1fms", update_time);
		gfx2D->DrawText(Vector2D(0.f, height - debugger_font->GetSize() * optionPosition), outputString, debugger_font);
		optionPosition++;
	}
	if(params[Parameter::CPU_TIME] == true){
		if(cpu_time == 0){
			gfx2D->DrawText(Vector2D(0, height - debugger_font->GetSize() * optionPosition), "Update Time: -", debugger_font);
		}else{
			sprintf(outputString, "CPU Time: %0.1fms", cpu_time);
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
}

void Debugger::SetCPUTime(float micro) {
	if(cpu_counter == 20){
		cpu_counter = 0;
		cpu_time = cpu_sum / 20.0f / 1000.0f;
		cpu_sum = 0;
	}else{
		cpu_sum += micro;
		cpu_counter++;
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
	return 1000.f / update_time;
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