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
#include "BaseTypes.h"

namespace cross {

class String {
public:
	String();
	String(const char* cstr);
	String(const String& str);
	String(S32 number);
	~String();

	U32 Length() const;
	const char* ToCStr() const;
	S32 ToInt() const;
	float ToFloat() const;

	String& operator = (const char* other);
	String& operator = (const String& other);

private:
	char* data = null;
	U32 length = 0;
};

}