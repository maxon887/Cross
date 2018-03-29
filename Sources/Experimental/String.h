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
	String(const Color& color);
	~String();

	/* Return lenth of the string. Number of placed characters spots */
	U32 Length() const;
	/* Return count of charactes that can be placed without string memory allocation */
	U32 Capacity() const;
	/* Cleans all charactes in this string without momory reallocation */
	void Clean();
	/* Returns true if string is empty */
	bool IsEmpty() const;
	/* Calculates has from current String */
	U64 Hash() const;
	/* Returns position of a first occurrence of char c in this String or -1 if wasn't found */
	S32 Find(char c) const;
	/* Returns position of a first occurrence of subStr in this String or -1 if wasn't found */
	S32 Find(const char* subStr) const;
	/* Returns position of a last occurrence of char in this String or -1 if wasn't found */
	S32 FindLast(char c) const;
	/* Returns first occurrence of any character in provided sequence */
	S32 FindFirstOf(const char* sequence) const;
	/* Returns first occurrence of any character in provided sequence started from specific position */
	S32 FindFirstOf(const char* sequence, U32 startPos) const;
	/* Returns first occurrence of not of any character in provided sequence */
	S32 FindNonFirstOf(const char* sequence) const;
	/* Returns first occurrence of not of any character in provided sequence started from specific position */
	S32 FindNonFirstOf(const char* sequence, U32 startPos) const;
	/* Converts this string to uppercase */
	void Uppercase();
	/* Converts this string to lowercase */
	void Lowercase();
	/* Removes first occurrence of subStr in current String */
	bool Remove(const char* subStr);
	/* Removes all characters before first argument and after last from current String */
	void Cut(U32 first, U32 last);
	/* Returns cut String from first character to last by arguments */
	String SubString(U32 first, U32 last) const;
	/* Returns data pointer on this String */
	char* ToCStr() const;
	/* Converts this String to integer type and returns it */
	S32 ToInt() const;
	/* Converts this String to integer type. Assumed that number storred in provided base */
	S32 ToInt(U32 base) const;
	/* Converts this String to float type and returns it */
	float ToFloat() const;
	/* Converts this String to Color object and returns it */
	Color ToColor() const;

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

	friend bool operator < (const String& left, const String& right);

private:
	char* data = nullptr;
	U32 length = 0;
	U32 capacity = 0;

	String(const char* cstr, U32 length, U32 capacity);
};


}