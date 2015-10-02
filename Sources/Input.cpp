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
	
#include "Input.h"
#include "Game.h"

using namespace cross;

void Input::TriggerActionDown(Point pos){
	global_mutex.lock();
	TRIGGER_EVENT(ActionDown, pos);
	global_mutex.unlock();
}

void Input::TriggerActionUp(Point pos){
	global_mutex.lock();
	TRIGGER_EVENT(ActionUp, pos);
	global_mutex.unlock();
}

void Input::TriggerActionMove(Point pos){
	global_mutex.lock();
	TRIGGER_EVENT(ActionMove, pos);
	global_mutex.unlock();
}

void Input::TriggerKeyPressed(Key key){
	global_mutex.lock();
	TRIGGER_EVENT(KeyPressed, key);
	global_mutex.unlock();
}

void Input::TriggerKeyReleased(Key key){
	global_mutex.lock();
	TRIGGER_EVENT(KeyReleased, key);
	global_mutex.unlock();
}
