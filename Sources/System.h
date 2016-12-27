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
#include "Libs/Events/Event.h"

namespace cross {
	
/* Class for platform specific functional */
class System{
public:
	/* Occurs when window size changed */
	DECLARE_EVENT(void, S32, S32) WindowResized;
	/* Print message in console. See LogIt() if you need formated output */
	virtual void Log(const char* msg) = 0;
	/* Returns path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* Returns path to the application data folder */
	virtual string DataPath() = 0;
	/* Return current time in microseconds */
	virtual U64 GetTime() = 0;

	System();
	virtual ~System() { }
	/* Loads specific file from game assets. */
	virtual File* LoadFile(const string& filename);
	/* Safe file to Data folder */
	virtual void SaveFile(File* file);
	/* Force current thread to sleep */
	virtual void Sleep(float milis) { };
	/* Ask user about exit from application */
	virtual void PromtToExit() { };
	/* Returns Commercial class object */
	virtual Commercial* GetCommercial() { return NULL; };
	/* Print message in console. */
	void LogIt(const char* format, ...);
	/* Returns window width in pixels. Don't miss wiht Screen::GetWidth() */
	S32 GetWindowWidth();
	/* Returns window Height in pixels. Don't miss wiht Screen::GetHeight() */
	S32 GetWindowHeight();
	/* Returns physical screen aspect ration */
	float GetAspectRatio();
	/* Returns directory from full filename */
	string PathFromFile(const string& filePath);
	/* Returns extencsion from filename or full file path */
	string ExtensionFromFile(const string& file);
	/* Returns raw file name without extencion (ex "log" for "log.txt") */
	string FileWithoutExtension(const string& file);
	/* Engine specific */
	void SetWindowSize(S32 width, S32 height);

private:
	S32 window_width;
	S32 window_height;
};
    
}
