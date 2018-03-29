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

namespace cross {

class String {
public:
	String();
	String(const char* cstr);
	String(const char* begin, const char* end);
	String(const String& str);
	String(S32 number);
	String(U32 number);
	String(float number);
	~String();

	U32 Length() const;
	U32 Capacity() const;
	/* Returns position of a first occurrence of subStr in this String or -1 if wasn't found */
	S32 Find(const char* subStr) const;
	S32 FindFirstOf(const char* sequence) const;
	S32 FindFirstOf(const char* sequence, U32 startPos) const;
	S32 FindNonFirstOf(const char* sequence) const;
	S32 FindNonFirstOf(const char* sequence, U32 startPos) const;
	void Uppercase();
	void Lowercase();
	void Remove(const char* subStr);
	void Cut(U32 first, U32 last);
	const char* ToCStr() const;
	S32 ToInt() const;
	float ToFloat() const;

	String& operator = (const char* other);
	String& operator = (const String& other);

	bool operator == (const char* other) const;
	bool operator != (const char* other) const;
	bool operator == (const String& other) const;
	bool operator != (const String& other) const;

	void operator += (const char* other);
	void operator += (const String& other);
	friend String operator + (const String& left, const char* right);
	friend String operator + (const char* left, const String& right);
	friend String operator + (const String& left, const String& right);
	template<class Convertable>
	friend String operator + (const String& left, const Convertable& right);

private:
	char* data = nullptr;
	U32 length = 0;
	U32 capacity = 0;

	String(const char* cstr, U32 length, U32 capacity);
};

template<class Convertable>
String operator + (const String& left, const Convertable& right) {
	return left + String(right);
}

}