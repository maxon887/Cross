#include "Debugger.h"
#include "System.h"

using namespace cross;

Debugger* Debugger::instance = nullptr;

Debugger* Debugger::Instance() {
	if(!instance) {
		instance = CREATE Debugger();
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
	return (float)(now - checkTime) / 1000.f;
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
