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

	Array(const Array& arr);
	Array();
	Array(unsigned int capacity);
	~Array();

	void Initialize(const Type& element);
	void PushBack(const Type& element);
	void PopBack();
	Type& Back();
	unsigned int Size() const;
	unsigned int Capacity() const;
	Type& Get(unsigned int index) const;
	Iterator Begin() const;
	Iterator End() const;
	void Clear();
	bool Empty() const;
	void Insert(Iterator begin, T* first, T* last);

	Type& operator [] (unsigned int index) const;
	void operator = (const Array<Type>& other);

	//back compatibility
	Iterator begin() const;
	Iterator end() const;
	void push_back(const Type& element);
	void pop_back();
	Type& back();
	unsigned int size() const;
	unsigned int capacity() const;
	void clear();
	void insert(Iterator begin, T* first, T* last);
private:
	Type* data;
	unsigned int mSize;
	unsigned int mCapacity;

	void CopyData(Type* data, unsigned int size);
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
Array<Type>::Array(const Array& arr) :
	mSize(arr.mSize),
	mCapacity(arr.mCapacity)
{
	data = new Type[mCapacity];
	CopyData(arr.data, arr.mSize);
}

template<class Type>
Array<Type>::Array() :
	Array(0)
{ }

template<class Type>
Array<Type>::Array(unsigned int capacity) :
	data(nullptr),
	mSize(capacity),
	mCapacity(capacity)
{
	if(mCapacity != 0){
		data = new Type[mCapacity];

		unsigned char* dataPtr = (unsigned char*)data;
		for(unsigned int i = 0; i < mCapacity * sizeof(Type); ++i){
			*(dataPtr + i) = (unsigned char)0;
		}
	}else{
		mCapacity = 1;
		data = new Type[mCapacity];
	}
}

template<class Type>
Array<Type>::~Array(){
	delete[] data;
}

template<class Type>
void Array<Type>::Initialize(const Type& element){
	for(unsigned int i = 0; i < mCapacity; ++i){
		data[i] = element;
	}
}

template<class Type>
void Array<Type>::PushBack(const Type& element){
	if(mSize >= mCapacity){
		mCapacity *= 2;
		Type* oldData = data;
		data = new Type[mCapacity];
		CopyData(oldData, mSize);
		delete[] oldData;
	}
	data[mSize] = element;
	mSize++;
}

template<class Type>
void Array<Type>::PopBack(){
	mSize--;
}

template<class Type>
Type& Array<Type>::Back(){
	return data[mSize - 1];
}

template<class Type>
unsigned int Array<Type>::Size() const{
	return mSize;
}

template<class Type>
unsigned int Array<Type>::Capacity() const{
	return mCapacity;
}

template<class Type>
Type& Array<Type>::Get(unsigned int index) const{
	if(index < mCapacity) {
		return data[index];
	} else {
		throw CrossException("Out of bounds exception");
	}
}

template<class Type>
Type& Array<Type>::operator[](unsigned int index) const{
	return Get(index);
}

template<class Type>
void Array<Type>::operator=(const Array<Type>& other){
	this->mCapacity = other.mCapacity;
	this->mSize = other.mSize;
	delete[] data;
	data = new Type[mCapacity];
	CopyData(other.data, mSize);
}

template<class Type>
typename Array<Type>::Iterator Array<Type>::Begin() const{
	return Iterator(data);
}

template<class Type>
typename Array<Type>::Iterator Array<Type>::End() const{
	return Iterator(data + mSize);
};

template<class Type>
void Array<Type>::Clear(){
	mSize = 0;
}

template<class Type>
bool Array<Type>::Empty() const{
	return mSize == 0;
}

//back compatibility
template<class Type> 
typename Array<Type>::Iterator Array<Type>::begin() const{
	return Begin();
}

template<class Type>
typename Array<Type>::Iterator Array<Type>::end() const{
	return End();
}

template<class Type>
void Array<Type>::push_back(const Type& element){
	PushBack(element);
}

template<class Type>
void Array<Type>::pop_back(){
	PopBack();
}

template<class Type>
Type& Array<Type>::back(){
	return Back();
}

template<class Type>
unsigned int Array<Type>::size() const{
	return Size();
}

template<class Type>
unsigned int Array<Type>::capacity() const{
	return Capacity();
}

template<class Type>
void Array<Type>::clear(){
	Clear();
}

//private members
template<class Type>
void Array<Type>::CopyData(Type* data, unsigned int size){
	for(unsigned int i = 0; i < size; ++i){
		this->data[i] = data[i];
	}
}

}