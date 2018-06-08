/*	Copyright © 2018 Maksim Lukyanov

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

namespace cross {

/*	Provides platform specific functional. Use this class for logging, file access and other platform specific function */
class System {
public:
	enum Orientation {
		LANDSCAPE,
		PORTRAIT,
		AUTO,
		COUNT
	};
	/* Occurs when window size changed */
	Event<S32, S32> WindowResized;
	/* Occurs when window orientation changed */
	Event<System::Orientation> OrientationChanged;

	virtual ~System() { }

	/* Print message in console. See LogIt() if you need formatted output */
	virtual void Log(const char* msg) = 0;
	/* Returns path to the application assets folder */
	virtual String AssetsPath() = 0;
	/* Returns path to the application data folder */
	virtual String DataPath() = 0;
	/* Return current time in microseconds */
	virtual U64 GetTime() = 0;

	/* Load file from absolute path */
	virtual File* LoadFile(const String& filename);
	/* Load file from game asset directory */
	virtual File* LoadAssetFile(const String& filename);
	/* Load file from data directory */
	virtual File* LoadDataFile(const String& filename);
	/* Save file by absolute folder */
	virtual void SaveFile(File* file);
	/* Save file to data folder */
	virtual void SaveDataFile(File* file);
	/* Checks if file exists in provided path */
	virtual bool IsFileExists(const String& filename);
	/* Checks if file exists in assets directory */
	virtual bool IsAssetFileExists(const String& filename);
	/* Checks if file exists in data directory */
	virtual bool IsDataFileExists(const String& filename);
	/* Checks if folder exists in provided path */
	virtual bool IsDirectoryExists(const String& filepath);
	/* Checks if folder exists in assets path */
	virtual bool IsAssetDirectoryExists(const String& filepath);
	/* Checks if folder exists in data path */
	virtual bool IsDataDirectoryExists(const String& filepath);
	/* Creates folder in provided path */
	virtual void CreateDirectory(const String& dirname);
	/* Returns all sub folders in provided directory */
	virtual Array<String> GetSubDirectories(const String& filepath);
	/* Returns all files contain in provided directory */
	virtual Array<String> GetFilesInDirectory(const String& filepath);
	/* Notifies user that something happened by system message. Returns true if system should skip this message in further use */
	virtual bool Alert(const String& msg);
	/* Shows system information message */
	virtual void Messagebox(const String& title, const String& msg);
	/* Force current thread to sleep */
	virtual void Sleep(float milis);
	/* Requests to set required orientation. Can not to be set if system not allow to change orientation */
	virtual void RequestOrientation(Orientation orientation);
	/* Returns true if current working platform is mobile one */
	virtual bool IsMobile() { return false; }
	/* Returns device screen DPI. This is approximate value. Real devices does not know their screen sizes */
	virtual float GetScreenDPI();
	/* Ask user about exit from application */
	virtual void PromtToExit() { }
	/* Obtains clipboard data as text if available */
	virtual String GetClipboard();
	/* Sets current String int clipboard buffer */
	virtual void SetClipboard(const String& data);
	/* Tries to open file with appropriate external program */
	virtual void OpenFileExternal(const String& filename);
	/* Opens system window for choosing file */
	virtual String OpenFileDialog(bool saveDialog = false);
	/* Prints String */
	template<class... Args>
	void LogIt(const String& msg, Args... args);
	/* Notifies user that something happened by system message. Usually something bad. Use it at last case */
	template<class... Args>
	void Alert(const String& message, const char* filename, U32 line, Args... args);
	/* How much screen elements must be increased or decreased due to device DPI */
	float GetScreenScale();
	/* Returns window width in pixels */
	S32 GetWindowWidth() const;
	/* Returns window height in pixels */
	S32 GetWindowHeight() const;
	/* Returns current device orientation */
	Orientation GetDeviceOrientation() const;
	/* Returns physical screen aspect ration */
	float GetAspectRatio() const;
	/* Sets physical device screen size */
	void SetWindowSize(S32 width, S32 height);
	/* Returns all messages that was logged. Works only in debug builds */
	String& GetLogBuffer();

private:
	S32 window_width	= -1;
	S32 window_height	= -1;
	Set<U64> asserts_hashes;

	String log_buffer;
};

template<class... Args>
void System::LogIt(const String& format, Args... args) {
	String formatted = String::Format(format, args...);
	Log(formatted);

#ifdef CROSS_DEBUG
	if(log_buffer.Length() > 8192) {
		log_buffer.Cut(1024, log_buffer.Length());
	}

	log_buffer += formatted;
	log_buffer += "\n";
#endif // CROSS_DEBUG
}

template<class... Args>
void System::Alert(const String& message, const char* filename, U32 line, Args... args) {
	U64 hash = message.Hash();
	auto found = asserts_hashes.find(hash);
	if(found == asserts_hashes.end()) {
		String formatted = String::Format(message, args...);
		formatted += "\n";
		formatted += "File: ";
		formatted += filename;
		formatted += "\n";
		formatted += "Line: ";
		formatted += line;

		String logged = "==========ASSERT!==========";
		logged += "\n";
		logged += formatted;
		logged += "\n";
		logged += "===========================";
		Log(logged);
#ifdef CROSS_DEBUG
		if(Alert(formatted)) {
			asserts_hashes.insert(hash);
		}
#else
		asserts_hashes.insert(hash);
#endif
	}
}

}
