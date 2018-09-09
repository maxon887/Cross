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
#include "Cross.h"
#include "System.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

using namespace cross;

String String::Format(const String& format) {
	S32 spot = format.Find('#');
	CROSS_ASSERT(spot == -1, "Formatter error. Less values provied than expected");
	return format;
}

String::String() :
	length(0),
	capacity(0)
{
	data = (char*)CROSS_ALLOC(capacity + 1);
	data[0] = '\0';
}

String::String(const char* cstr) {
	CROSS_ASSERT(cstr, "Attempt to create a String from null pointer");
	length = strlen(cstr);
	capacity = length;
	data = (char*)CROSS_ALLOC(capacity + 1);
	memcpy(data, cstr, (Size)length + 1);
}

String::String(const char* cstr, S32 len, S32 cap) :
	length(len),
	capacity(cap)
{
	CROSS_ASSERT(cstr, "Attempt to create a String from null pointer");
	data = (char*)CROSS_ALLOC(capacity + 1);
	memcpy(data, cstr, (Size)length + 1);
}

String::String(const char* begin, const char* end) {
	const char* iter = begin;
	while(iter != end) {
		length++;
		iter++;
	}
	capacity = length;
	data = (char*)CROSS_ALLOC(capacity + 1);
	memcpy(data, begin, (Size)length + 1);
	data[length] = '\0';
}

String::String(const String& str) :
	length(str.length),
	capacity(str.capacity)
{
	data = (char*)CROSS_ALLOC(capacity + 1);
	memcpy(data, str.data, (Size)str.length + 1);
}

String::String(String&& str) :
	length(str.length),
	capacity(str.capacity),
	data(str.data)
{
	str.length = 0;
	str.capacity = 0;
	str.data = nullptr;
}

String::String(S32 number) : String(number, "%d", 12) { }

String::String(U32 number) : String(number, "%u", 11) { }

String::String(S64 number) : String(number, "%lld", 50) { }

String::String(U64 number) : String(number, "%llu", 50) { }

String::String(float number) : String(number, "%f", 50) { }

String::~String() {
	CROSS_FREE(data);
}

S32 String::Length() const {
	return length;
}

S32 String::Capacity() const {
	return capacity;
}

void String::Clear() {
	length = 0;
	*data = '\0';
}

bool String::IsEmpty() const {
	return length == 0;
}

U64 String::Hash() const {
	U64 hash = 5381;
	int c;
	char* str = data;
	while((c = *str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

S32 String::Find(char c) const {
	char* result = strchr(data, c);
	if(result) {
		return (S32)(result - data);
	} else {
		return -1;
	}
}

S32 String::Find(const char* subStr) const {
	return Find(subStr, 0);
}

S32 String::Find(const char* subStr, S32 startPos) const {
	char* result = strstr(data + startPos, subStr);
	if(result) {
		return (S32)(result - data);
	} else {
		return -1;
	}
}

S32 String::FindLast(char c) const {
	char* result = strrchr(data, c);
	if(result) {
		return (S32)(result - data);
	} else {
		return -1;
	}
}

S32 String::FindFirstOf(const char* sequence) const {
	return FindFirstOf(sequence, 0);
}

S32 String::FindFirstOf(const char* sequence, S32 startPos) const {
	S32 occurrence = strcspn(data + startPos, sequence);
	if(occurrence == length) {
		return -1;
	}
	return startPos + occurrence;
}

S32 String::FindNonFirstOf(const char* sequence) const {
	return FindNonFirstOf(sequence, 0);
}

S32 String::FindNonFirstOf(const char* sequence, S32 startPos) const {
	U32 occurence = strspn(data + startPos, sequence);
	if(occurence == length) {
		return -1;
	}
	return startPos + occurence;
}

void String::Uppercase() {
	char* start = data;
	while(*start) {
		*start = (char)toupper(*start);
		start++;
	}
}

void String::Lowercase() {
	char* start = data;
	while(*start) {
		*start = (char)tolower(*start);
		start++;
	}
}

bool String::Remove(const char* subStr) {
	return Remove(subStr, 0);
}

bool String::Remove(const char* subStr, S32 startPos) {
	char* begin = strstr(data + startPos, subStr);
	if(begin) {
		U32 cLen = strlen(subStr);
		U32 size = length - (begin - data);
		memcpy(begin, begin + cLen, size + 1);
		length -= cLen;
		return true;
	}
	return false;
}

bool String::Remove(char c) {
	S32 res = Find(c);
	if(res != -1) {
		memcpy(data + res, data + res + 1, (Size)length - res);
		length--;
		return true;
	}
	return false;
}

bool String::Replace(const char* from, const char* to) {
	bool result = false;
	S32 spot = Find(from);
	S32 offset = 0;
	String replaceTo = to;
	while(spot != -1) {
		result = true;
		Remove(from, offset);
		Insert(spot, replaceTo);
		offset = spot + replaceTo.Length();
		spot = Find(from, offset);
	}
	return result;
}

void String::Cut(S32 first, S32 last) {
	length = last - first;
	memcpy(data, data + first, (Size)length);
	data[length] = '\0';
}

void String::Insert(S32 pos, const String& str) {
	S32 newLen = length + str.length;
	if(capacity < newLen) {
		data = (char*)CROSS_REALLOC(data, newLen + 1);
		capacity = newLen;
	}
	String temp = SubString(pos, length);
	memcpy(data + pos, str.ToCStr(), (Size)str.length);
	memcpy(data + pos + str.length, temp.ToCStr(), (Size)temp.length + 1);
	length += str.length;
}

String String::SubString(S32 first, S32 last) const {
	S32 len = last - first;
	String result("", len, len);
	char* newData = result.ToCStr();
	memcpy(newData, data + first, (Size)len);
	newData[len] = '\0';
	return result;
}

char* String::ToCStr() const {
	return data;
}

S32 String::ToInt() const {
	return ToInt(10);
}

S32 String::ToInt(U32 base) const {
	char* endp;
	S32 value = strtol(data, &endp, base);
	CROSS_RETURN(endp != data, 0, "Conversion from string '#' to integer failed", data);
	CROSS_ASSERT(*endp == '\0', "String '#' contains unrecognized symbols. Conversion result may be unexpected", data);
	return value;
}

float String::ToFloat() const {
	char* endp;
	float value = strtof(data, &endp);
	CROSS_RETURN(endp != data, 0, "Conversion from string '#' to float failed", data);
	CROSS_ASSERT(*endp == '\0', "String '#' contains unrecognized symbols. Conversion result may be unexpected", data);
	return value;
}

String& String::operator = (const char* cstr) {
	S32 cLen = strlen(cstr);
	if(capacity < cLen) {
		data = (char*)CROSS_REALLOC(data, cLen + 1);
		capacity = cLen;
	}
	length = cLen;
	memcpy(data, cstr, (Size)length + 1);
	return *this;
}

String& String::operator = (const String& other) {
	if(capacity < other.length) {
		data = (char*)CROSS_REALLOC(data, other.length + 1);
		capacity = other.length;
	}
	length = other.length;
	memcpy(data, other.data, (Size)other.length + 1);
	return *this;
}

String& String::operator = (String&& other) {
	CROSS_FREE(data);
	data = other.data;
	length = other.length;
	capacity = other.capacity;
	other.data = nullptr;
	other.length = 0;
	other.capacity = 0;
	return *this;
}

bool String::operator == (const char* other) const {
	return strcmp(data, other) == 0;
}

bool String::operator != (const char* other) const {
	return !(*this == other);
}

bool String::operator == (const String& other) const {
	return strcmp(data, other.data) == 0;
}

bool String::operator != (const String& other) const {
	return !(*this == other);
}

void String::operator += (const char* other) {
	S32 otherLen = strlen(other);
	S32 requiredLen = length + otherLen;
	if(requiredLen > capacity) {
		data = (char*)CROSS_REALLOC(data, requiredLen + 1);
		capacity = requiredLen;
	}
	memcpy(data + length, other, (Size)otherLen + 1);
	length = requiredLen;
}

void String::operator += (const String& other) {
	S32 requiredLen = length + other.length;
	if(requiredLen > capacity) {
		data = (char*)CROSS_REALLOC(data, requiredLen + 1);
		capacity = requiredLen;
	}
	memcpy(data + length, other.data, (Size)other.length + 1);
	length = requiredLen;
}

String::operator char * () {
	return data;
}

String::operator const char * () const {
	return data;
}

namespace cross {

String operator + (const String& left, const char* right) {
	U32 cstrLen = strlen(right);
	U32 capacity = cstrLen + left.length;
	String result(left.ToCStr(), left.length, capacity);
	result += right;
	return result;
}

String operator + (const char* left, const String& right) {
	U32 cstrLen = strlen(left);
	U32 capacity = cstrLen + right.length;
	String result(left, cstrLen, capacity);
	result += right;
	return result;
}

String operator + (const String& left, const String& right) {
	S32 capacity = left.length + right.length;
	String result(left.ToCStr(), left.length, capacity);
	result += right;
	return result;
}

bool operator < (const String& left, const String& right) {
	S32 res = strcmp(left.ToCStr(), right.ToCStr());
	return res < 0;
}

}
