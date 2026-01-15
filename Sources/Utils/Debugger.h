#pragma once
#include "Cross.h"
#include "Base/Array.h"

namespace cross{

class Debugger {
public:
	static Debugger* Instance();
	static void Release();

	void Update(float micro);

	void SetTimeCheck();
	float GetTimeCheck();
	void SetCPUTime(float sec);
	float GetCPUTime() const;
	float GetUpdateTime() const;
	float GetFPS() const;

private:
	static Debugger* instance;

	Array<U64> time_checks			= Array<U64>();

	float cpu_time					= 0;
	float cpu_sum					= 0;
	int cpu_counter					= 0;

	float update_time				= 0;
	float update_sum				= 0;
	int update_counter				= 0;

	Debugger() = default;
	~Debugger() = default;
};
	
}
