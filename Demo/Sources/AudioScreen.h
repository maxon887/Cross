#pragma once
#include "Screen.h"

using namespace cross;

class AudioScreen :	public Screen {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Sound* bck_music		= nullptr;
	Sound* jaguar			= nullptr;
	Sound* truck			= nullptr;
	bool is_bck_playing		= false;
};

