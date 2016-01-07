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

#ifdef _WIN32
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#define IOS true
#endif

namespace cross {

class Commercial;

/* Abstract class contained platform specific intrface */
class Launcher{
public:
	/* Returns physical screen width in pixels */
	virtual int GetTargetWidth() = 0;
	/* Returns physical screen height in pixels */
	virtual int GetTargetHeight() = 0;
	/* Log out message */
	virtual void LogIt(string msg) = 0;
	/* return path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* return path to the application data folder */
	virtual string DataPath() = 0;
	/* Loads specific file from game assets. */
	virtual unsigned char* LoadFile(string filename, int *size) = 0;
	virtual File* LoadFile(string filename) = 0;
	virtual void Sleep(float milis) { };
	virtual void PromtToExit() { };

	void LogIt(int msg);
	void LogIt(float msg);
	float DeviceAspect();

	virtual Commercial* GetCommercial() { return NULL; };
//Internal data. You don't need call any of this methods or modify variable
public:
	virtual ~Launcher() { }
private:
};
    
}
