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
	
#pragma once

#include "Game.h"
#include "Commercial.h"

using namespace cross;

enum Control{
	SLIDE,
	ARROWS
};

class JimTheSnake : public Game{
public:
	JimTheSnake(Launcher* launcher);
	Screen* GetStartScreen();
	void Start();

	bool IsMusicEnabled();
	bool IsSoundEnabled();
	Control GetControl();
	bool IsPurchased();
	int BestScore();
	void SetMusicEnabled(bool enabled);
	void SetSoundEnabled(bool enabled);
	void SetControl(Control control);
	void SetBestScore(int best);
	Commercial* GetCommercial();
private:
	Commercial* commercial;
	bool music;
	bool sound;
	Control control;
	bool purchased;
	int score;
	void CommercialCallback(Commercial::Event e);
};
