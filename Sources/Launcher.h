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
#include "Cross.h"

namespace cross {

class Commercial;

/* Abstract class contained platform specific functionality. */
class Launcher{
public:
	/* Returns physical screen width in pixels */
	virtual int GetTargetWidth() = 0;
	/* Returns physical screen height in pixels */
	virtual int GetTargetHeight() = 0;
	/* Cross platform way to log out message */
	virtual void LogIt(const string& msg) = 0;
	virtual void LogIt(const char* formatStr, ...) = 0;
	/* return path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* return path to the application data folder */
	virtual string DataPath() = 0;
	/* Loads specific file from game assets. */
	virtual File* LoadFile(const string& filename) = 0;
	virtual void Sleep(float milis) { };
	virtual void PromtToExit() { };
	/* Return physical screen aspec ration */
	float GetAspectRatio();
	string PathFromFile(const string& filePath);

	virtual Commercial* GetCommercial() { return NULL; };
//Internal data. You don't need call any of this methods or modify variable
public:
	virtual ~Launcher() { }
private:
};
    
}
