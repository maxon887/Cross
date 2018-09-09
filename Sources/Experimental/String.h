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

#include <stdio.h>

namespace cross {

class String {
public:
	static String Format(const String& format);
	template<class First, class... Args>
	static String Format(const String& format, First value, Args... args);

	String();
	String(const char* cstr);
	String(const char* begin, const char* end);
	String(const String& str);
	String(String&& str);
	String(S32 number);
	String(U32 number);
	String(S64 number);
	String(U64 number);
	String(float number);
	String(const char* cstr, S32 length, S32 capacity);
	template<class Value>
	String(Value value, const char* format, S32 bufferSize);
	~String();

	/* Return length of the string. Number of placed characters spots */
	S32 Length() const;
	/* Return count of characters that can be placed without string memory allocation */
	S32 Capacity() const;
	/* Cleans all characters in this string without memory reallocation */
	void Clear();
	/* Returns true if string is empty */
	bool IsEmpty() const;
	/* Calculates has from current String */
	U64 Hash() const;
	/* Returns position of a first occurrence of char c in this String or -1 if wasn't found */
	S32 Find(char c) const;
	/* Returns position of a first occurrence of subStr in this String or -1 if wasn't found */
	S32 Find(const char* subStr) const;
	/* Returns position of a first occurrence of subStr in this String or -1. start from startPos */
	S32 Find(const char* subStr, S32 startPos) const;
	/* Returns position of a last occurrence of char in this String or -1 if wasn't found */
	S32 FindLast(char c) const;
	/* Returns first occurrence of any character in provided sequence */
	S32 FindFirstOf(const char* sequence) const;
	/* Returns first occurrence of any character in provided sequence started from specific position */
	S32 FindFirstOf(const char* sequence, S32 startPos) const;
	/* Returns first occurrence of not of any character in provided sequence */
	S32 FindNonFirstOf(const char* sequence) const;
	/* Returns first occurrence of not of any character in provided sequence started from specific position */
	S32 FindNonFirstOf(const char* sequence, S32 startPos) const;
	/* Converts this string to uppercase */
	void Uppercase();
	/* Converts this string to lowercase */
	void Lowercase();
	/* Removes first occurrence of subStr in current String */
	bool Remove(const char* subStr);
	/* Removes first occurrence of subStr in current String start with pos*/
	bool Remove(const char* subStr, S32 startPos);
	/* Removes first occurrence of character in current String */
	bool Remove(char c);
	/* Replaces all occurrences of 'from' string to 'to' string */
	bool Replace(const char* from, const char* to);
	/* Removes all characters before first argument and after last from current String */
	void Cut(S32 first, S32 last);
	/* Inserts provided String in the middle of current string at position */
	void Insert(S32 position, const String& str);
	/* Returns cut String from first character to last by arguments */
	String SubString(S32 first, S32 last) const;
	/* Returns data pointer on this String */
	char* ToCStr() const;
	/* Converts this String to integer type and returns it */
	S32 ToInt() const;
	/* Converts this String to integer type. Assumed that number stored in provided base */
	S32 ToInt(U32 base) const;
	/* Converts this String to float type and returns it */
	float ToFloat() const;

	String& operator = (const char* other);
	String& operator = (const String& other);
	String& operator = (String&& other);

	bool operator == (const char* other) const;
	bool operator != (const char* other) const;
	bool operator == (const String& other) const;
	bool operator != (const String& other) const;

	void operator += (const char* other);
	void operator += (const String& other);

	operator char* ();
	operator const char* () const;

	friend String operator + (const String& left, const char* right);
	friend String operator + (const char* left, const String& right);
	friend String operator + (const String& left, const String& right);

	friend bool operator < (const String& left, const String& right);

private:
	char* data = nullptr;
	S32 length = 0;
	S32 capacity = 0;
};

template<class First, class... Args>
String String::Format(const String& format, First value, Args... args) {
	S32 spot = format.Find('#');
	//More values provided than expected
	assert(spot != -1);
	String result = format;
	result.Remove('#');
	result.Insert(spot, String(value));
	return Format(result, args...);
}

template<class Value>
String::String(Value value, const char* format, S32 bufferSize) {
	data = (char*)CROSS_ALLOC(bufferSize + 1);
	length = sprintf(data, format, value);
	capacity = bufferSize;

	//Conversion from integer to string failed
	assert(length > 0);
	//More data written in buffer than was allocated
	assert(length < bufferSize);
}

}
