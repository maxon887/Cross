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
#include "System.h"
#include "Input.h"
#include "Camera2D.h"
#include "Game.h"
#include "Screen.h"

using namespace cross;

Debugger* Debugger::instance = NULL;

Debugger* Debugger::Instance(){
	if(!instance){
		instance = new Debugger();
	}
	return instance;
}

void Debugger::Release(){
	delete instance;
	instance = NULL;
}

Debugger::Debugger() {
	params[FPS]			= true;
	params[UPDATE_TIME]	= true;
	params[CPU_TIME]	= true;
	params[RUN_TIME]	= true;
	params[INPUT]		= true;

	if(params[Parameter::INPUT]){
		input->ActionDown.Connect(this, &Debugger::OnActionDown);
		input->ActionMove.Connect(this, &Debugger::OnActionMove);
		input->ActionUp.Connect(this, &Debugger::OnActionUp);
	}
}

Debugger::~Debugger()
{ }

void Debugger::SetTimeCheck() {
	U64 checkTime = system->GetTime();
	time_checks.push_back(checkTime);
}

float Debugger::GetTimeCheck() {
	U64 now = system->GetTime();
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

float Debugger::GetFPS() const{
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