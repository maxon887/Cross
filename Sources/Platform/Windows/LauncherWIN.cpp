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

LauncherWIN::LauncherWIN(){
	AudioWIN::Init();
}

int LauncherWIN::GetTargetWidth(){
	return TARGET_WIDTH;
}

int LauncherWIN::GetTargetHeight(){
	return TARGET_HEIGHT;
}

string LauncherWIN::AssetsPath(){
	return "../../Assets/";
}

string LauncherWIN::DataPath(){
	return "../../Data/";
}

void LauncherWIN::LogIt(string msg){
	msg += "\n";
	OutputDebugString(msg.c_str());
}

Sound* LauncherWIN::CreateSound(string filename, bool loop){
	filename = AssetsPath() + filename;
	return new AudioWIN(filename, loop, false);
}

Music* LauncherWIN::CreateMusic(string filename, bool loop){
	filename = AssetsPath() + filename;
	return new AudioWIN(filename, loop, true);
}

void LauncherWIN::ShowMessage(string msg){
	MessageBox(wnd, msg.c_str(), "Exception", MB_OK);
}

void LauncherWIN::SetHWND(HWND wnd){
	this->wnd = wnd;
}