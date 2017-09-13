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
#include "Event.h"

#define CROSS_ASSERT(condition, message, ...)						\
if(!(condition)) {													\
	sys->Assert(__FILE__, __LINE__, message, ##__VA_ARGS__);		\
}

#define CROSS_FAIL(condition, message, ...)							\
if(!(condition)) {													\
	sys->Assert(__FILE__, __LINE__, message, ##__VA_ARGS__);		\
	return;															\
}

#define CROSS_RETURN(condition, value, message, ...)				\
if(!(condition)) {													\
	sys->Assert(__FILE__, __LINE__, message, ##__VA_ARGS__);		\
	return value;													\
}

#define CROSS_EXCEPTION(condition, message, ...)					\
if(!(condition)) {													\
	throw CrossException(message, ##__VA_ARGS__);					\
}

namespace cross {
	
/* Class for platform specific functional */
class System{
public:
	enum Orientation{
		LANDSCAPE,
		PORTRAIT,
		AUTO,
		COUNT
	};
	/* Occurs when window size changed */
	Event<S32, S32> WindowResized;
	/* Occurs when window orintaton changed */
	Event<System::Orientation> OrientationChanged;
	/* Print message in console. See LogIt() if you need formated output */
	virtual void Log(const char* msg) = 0;
	/* Returns path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* Returns path to the application data folder */
	virtual string DataPath() = 0;
	/* Return current time in microseconds */
	virtual U64 GetTime() = 0;

	virtual ~System() { }

	/* Load file from absolute path */
	virtual File* LoadFile(const string& filename);
	/* Load file from game asset directory */
	virtual File* LoadAssetFile(const string& filename);
	/* Load file from data directory */
    virtual File* LoadDataFile(const string& filename);
	/* Save file by absolute folder */
	virtual void SaveFile(File* file);
	/* Save file to data folder */
	virtual void SaveDataFile(File* file);
	/* Show sync messagebox(platform dependet) */
	virtual void Messagebox(const string& title, const string& msg);
	/* Force current thread to sleep */
	virtual void Sleep(float milis) { };
	/* Ask user about exit from application */
    virtual void RequestOrientation(Orientation orientation) { };
	virtual void PromtToExit() { };
	/* Print message in console. */
	void LogIt(const char* format, ...);
	void LogIt(const string& msg);
	void LogIt(const Vector3D& vector);
	/* Returns window width in pixels. Don't miss wiht Screen::GetWidth() */
	S32 GetWindowWidth() const;
	/* Returns window Height in pixels. Don't miss wiht Screen::GetHeight() */
	S32 GetWindowHeight() const;
    Orientation GetDeviceOrientation() const;
	/* Returns physical screen aspect ration */
	float GetAspectRatio() const;
	/* Engine specific */
	void SetWindowSize(S32 width, S32 height);
	/* Trigger assertion message if condition if false) */
	void Assert(const char* filename, unsigned int line, const char* msg, ...);

private:
	S32 window_width	= -1;
	S32 window_height	= -1;
	Set<int> asserts_hashes;
};
    
}
