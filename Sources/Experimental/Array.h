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
	Array(S32 size, const T& defaultValue);
	template<class... Args>
	Array(Args... args);
	~Array();

	void Add(const T& item);
	S32 Size() const;
	S32 Capacity() const;
	void Clear();
	T* GetData();

	T* begin();
	T* end();

	T& operator [] (S32 index);

private:
	S32 size		= 0;
	S32 capacity	= 0;
	T* data			= nullptr;
};

//implementation
#pragma push_macro("new")
#undef new

template<class T>
Array<T>::Array()
{ }

template<class T>
template<class... Args>
Array<T>::Array(Args... args) {
	for(auto&& x : { args... }) {
		Add(x);
	}
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
	CROSS_FREE(data);
}

template<class T>
void Array<T>::Add(const T& item) {
	if(capacity == 0) {
		capacity = 1;
		data = (T*)CROSS_ALLOC(sizeof(T));
	}
	if(size >= capacity) {
		capacity *= 2;
		data = (T*)CROSS_REALLOC(data, sizeof(T) * capacity);
	}
	new(data + size) T(item);
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
void Array<T>::Clear() {
	size = 0;
}

template<class T>
T* Array<T>::GetData() {
	return data;
}

template<class T>
T* Array<T>::begin() {
	return data;
}

template<class T>
T* Array<T>::end() {
	return data + size;
}

template<class T>
T& Array<T>::operator [] (S32 index) {
	assert(index < size && index >= 0 && "index out of bounds");
	return *(data + index);
}

#pragma pop_macro("new")

}