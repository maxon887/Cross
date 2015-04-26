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

/*	Class responsible for playing stream music.
	long background music files must become object of Music class */
class Music{
public:
	/* Start play music file */
	virtual void Play() = 0;
	/* Suspend play */
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual bool IsPlaying() = 0;
	/* Release all data related for Music class */
	virtual ~Music() { };
};