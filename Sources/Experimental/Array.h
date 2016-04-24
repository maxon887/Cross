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

namespace cross{

template<class Type>
class Array{
public:
	class Iterator {
	public:
		Iterator(Type* obj);

		bool operator == (const Iterator& it) const;
		bool operator != (const Iterator& it) const;
		void operator ++ ();
		Type& operator * () const;
	private:
		Type* p;
	};

	Array();
	~Array();

	void PushBack(Type element);
	unsigned int Size() const;
	Type Get(unsigned int index) const;
	Iterator Begin() const;
	Iterator End() const;

	Type operator [] (unsigned int index) const;

	//back compatibility
	Iterator begin() const;
	Iterator end() const;
private:
	Type* data;
	unsigned int size;
	unsigned int capacity;
};

template<class Type>
Array<Type>::Array() :
	data(nullptr),
	size(0),
	capacity(1)
{
	data = new Type[1];
};

//class Iterator
template<class Type>
Array<Type>::Iterator::Iterator(Type* obj){
	this->p = obj;
};

template<class Type>
bool Array<Type>::Iterator::operator==(const Iterator& it) const{
	return this->p == it.p;
}

template<class Type>
bool Array<Type>::Iterator::operator!=(const Iterator& it) const{
	return !(*this == it);
}

template<class Type>
void Array<Type>::Iterator::operator++(){
	this->p++;
}

template<class Type>
Type& Array<Type>::Iterator::operator*() const{
	return *p;
}

//class Array
template<class Type>
Array<Type>::~Array(){
	delete[] data;
}

template<class Type>
void Array<Type>::PushBack(Type element){
	if(size >= capacity){
		capacity *= 2;
		Type* newData = new Type[capacity];
		memcpy(newData, data, sizeof(Type) * size);
		delete[] data;
		data = newData;
	}
	data[size] = element;
	size++;
};

template<class Type>
unsigned int Array<Type>::Size() const{
	return size;
}

template<class Type>
Type Array<Type>::Get(unsigned int index) const{
	if(index < size) {
		return data[index];
	} else {
		throw CrossException("Out of bounds exception");
	}
}

template<class Type>
Type Array<Type>::operator[](unsigned int index) const{
	return Get(index);
};

template<class Type>
typename Array<Type>::Iterator Array<Type>::Begin() const{
	return Iterator(data);
};

template<class Type>
typename Array<Type>::Iterator Array<Type>::End() const{
	return Iterator(data + size);
};

//back compatibility
template<class Type> 
typename Array<Type>::Iterator Array<Type>::begin() const{
	return Begin();
};

template<class Type>
typename Array<Type>::Iterator Array<Type>::end() const{
	return End();
};

}