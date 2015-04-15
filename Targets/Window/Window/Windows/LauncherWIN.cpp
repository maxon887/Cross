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
	
#include "LauncherWIN.h"

LauncherWIN::LauncherWIN(HWND wnd){
	this->wnd = wnd;
}

int LauncherWIN::GetTargetWidth(){
	return TARGET_WIDTH;
}

int LauncherWIN::GetTargetHeight(){
	return TARGET_HEIGHT;
}

const char* LauncherWIN::DataPath(){
	return "../../../Games/Demo/Data";
}

void LauncherWIN::LogIt(const char* str){
	sprintf(str_buffer, "%s\n", str);
    OutputDebugString(str_buffer);
}

void LauncherWIN::ShowMessage(const char* msg){
	MessageBox(wnd, msg, "Exception", MB_OK);
}