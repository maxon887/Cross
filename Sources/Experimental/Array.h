/*	Copyright � 2018 Maksim Lukyanov

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

template<class T>
class Array {
public:
	Array();
	Array(const Array<T>& other);
	Array(S32 size, const T& defaultValue);
	~Array();

	void Add(const T& item);
	template<class... Args>
	void CreateInside(Args... args);
	S32 Size() const;
	S32 Capacity() const;
	S32 Find(const T& ref) const;
	void Reserve(S32 count);
	void Clear();
	T& Last();
	void RemoveLast();
	T* GetData();

	T* begin() const;
	T* end() const;

	T& operator [] (S32 index);
	const T& operator [] (S32 index) const;
	void operator = (const Array<T>& other);

private:
	S32 size		= 0;
	S32 capacity	= 0;
	T* data			= nullptr;

	void ReallocateIfNeeded();
	void Copy(const Array<T>& other);
};

//implementation
#pragma push_macro("new")
#undef new

template<class T>
Array<T>::Array()
{ }

template<class T>
Array<T>::Array(const Array<T>& other) {
	Copy(other);
}

template<class T>
Array<T>::Array(S32 s, const T& defaultValue) {
	size = s;
	capacity = s;
	data = (T*)CROSS_ALLOC(sizeof(T) * capacity);
	for(S32 i = 0; i < size; i++) {
		new(data + i) T(defaultValue);
	}
}

template<class T>
Array<T>::~Array() {
	for(S32 i = 0; i < size; i++) {
		T* obj = data + i;
		obj->~T();
	}
	if(data) {
		CROSS_FREE(data);
	}
}

template<class T>
void Array<T>::Add(const T& item) {
	ReallocateIfNeeded();
	new(data + size) T(item);
	size++;
}

template<class T>
template<class... Args>
void Array<T>::CreateInside(Args... args) {
	ReallocateIfNeeded();
	new(data + size) T(args...);
	size++;
}

template<class T>
S32 Array<T>::Size() const {
	return size;
}

template<class T>
S32 Array<T>::Capacity() const {
	return capacity;
}

template<class T>
S32 Array<T>::Find(const T& ref) const {
	for(S32 i = 0; i < size; i++) {
		if(data[i] == ref) {
			return i;
		}
	}
	return -1;
}

template<class T>
void Array<T>::Reserve(S32 count) {
	if(count > capacity) {
		if(capacity == 0) {
			data = (T*)CROSS_ALLOC(sizeof(T) * count);
		} else {
			data = (T*)CROSS_REALLOC(data, sizeof(T) * count);
		}
		capacity = count;
	}
}

template<class T>
void Array<T>::Clear() {
	for(S32 i = 0; i < size; i++) {
		T* obj = data + i;
		obj->~T();
	}
	size = 0;
}

template<class T>
T& Array<T>::Last() {
	return *(data + size - 1);
}

template<class T>
void Array<T>::RemoveLast() {
	size--;
}

template<class T>
T* Array<T>::GetData() {
	return data;
}

template<class T>
T* Array<T>::begin() const {
	return data;
}

template<class T>
T* Array<T>::end() const {
	return data + size;
}

template<class T>
T& Array<T>::operator [] (S32 index) {
	assert(index < size && index >= 0 && "index out of bounds");
	return *(data + index);
}

template<class T>
const T& Array<T>::operator [] (S32 index) const {
	assert(index < size && index >= 0 && "index out of bounds");
	return *(data + index);
}

template<class T>
void Array<T>::operator = (const Array<T>& other) {
	Copy(other);
}

template<class T>
void Array<T>::ReallocateIfNeeded() {
	if(capacity == 0) {
		capacity = 1;
		data = (T*)CROSS_ALLOC(sizeof(T));
	}
	if(size >= capacity) {
		capacity *= 2;
		data = (T*)CROSS_REALLOC(data, sizeof(T) * capacity);
	}
}

template<class T>
void Array<T>::Copy(const Array<T>& other) {
	Reserve(other.Size());
	for(S32 i = 0; i < other.Size(); i++) {
		Add(other[i]);
	}
}

#pragma pop_macro("new")

}