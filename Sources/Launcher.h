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
	virtual U32 GetTargetWidth() = 0;
	/* Returns physical screen height in pixels */
	virtual U32 GetTargetHeight() = 0;
	// TODO remove virtualization
	virtual void LogIt(const string& msg) = 0;
	/* Cross platform way to log out message */
	virtual void LogIt(const char* formatStr, ...) = 0;
	/* return path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* return path to the application data folder */
	virtual string DataPath() = 0;
	/* Loads specific file from game assets. */
	virtual File* LoadFile(const string& filename) = 0;
	/* Return current time in microseconds */
	virtual U64 GetTime() = 0;
	/* Force current thread sleep m miliseconds */
	virtual void Sleep(float milis) { };
	/* Ask user about exit from application */
	virtual void PromtToExit() { };
	/* Returns platform specific Commercial class object */
	virtual Commercial* GetCommercial() { return NULL; };
	/* Safe file to Data folder */
	void SaveFile(File* file);
	/* Returns physical screen aspec ration */
	float GetAspectRatio();
	/* Returns directory from full filename */
	string PathFromFile(const string& filePath);
	string ExtensionFromFile(const string& file);
	string FileWithoutExtension(const string& file);
//Internal data. You don't need call any of this methods or modify variable
public:
	virtual ~Launcher() { }
private:
};
    
}
