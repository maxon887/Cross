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
#include "String.h"

#include <cstring>
#include <stdlib.h>

using namespace cross;

String::String() : String("") { }

String::String(const char* cstr) {
	length = strlen(cstr);
	data = new char[length + 1];
	memcpy(data, cstr, length + 1);
}

String::String(const String& str) :
	length(str.length)
{
	data = new char[length + 1];
	memcpy(data, str.data, str.length + 1);
}

String::String(S32 number) {
	U32 digits = 0;
	if(number < 0) digits++;
	S32 val = number;
	while(val) {
		val /= 10;
		digits++;
	}
	data = new char[digits + 1];
	length = digits;
	_itoa(number, data, 10);
}

String::~String() {
	delete[] data;
}

U32 String::Length() const {
	return length;
}

const char* String::ToCStr() const {
	return data;
}

S32 String::ToInt() const {
	return atoi(data);
}

float String::ToFloat() const {
	return atof(data);
}

String& String::operator = (const char* cstr) {
	S32 cLen = strlen(cstr);
	if(length < cLen) {
		delete[] data;
		data = new char[cLen + 1];
	}
	length = cLen;
	memcpy(data, cstr, length + 1);
	return *this;
}

String& String::operator = (const String& other) {
	if(length < other.length) {
		delete[] data;
		data = new char[other.length + 1];
	}
	length = other.length;
	memcpy(data, other.data, other.length + 1);
	return *this;
}