/*	Copyright © 2018 Maksim Lukyanov

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

using namespace cross;

Debugger* Debugger::instance = nullptr;

Debugger* Debugger::Instance() {
	if(!instance) {
		instance = new Debugger();
	}
	return instance;
}

void Debugger::Release() {
	delete instance;
	instance = nullptr;
}

void Debugger::SetTimeCheck() {
	U64 checkTime = os->GetTime();
	time_checks.Add(checkTime);
}

float Debugger::GetTimeCheck() {
	U64 now = os->GetTime();
	U64 checkTime = time_checks.Last();
	time_checks.RemoveLast();
	return (now - checkTime) / 1000.f;
}

void Debugger::Update(float micro) {
	if(update_counter == 20) {
		update_counter = 0;
		update_time = update_sum / 20.f / 1000.f;
		update_sum = 0;
	} else {
		update_sum += micro;
		update_counter++;
	}
}

void Debugger::SetCPUTime(float micro) {
	if(cpu_counter == 20) {
		cpu_counter = 0;
		cpu_time = cpu_sum / 20.0f / 1000.0f;
		cpu_sum = 0;
	} else {
		cpu_sum += micro;
		cpu_counter++;
	}
}

float Debugger::GetCPUTime() const {
	return cpu_time;
}

float Debugger::GetUpdateTime() const {
	return update_time;
}

float Debugger::GetFPS() const {
	return 1000.f / update_time;
}
