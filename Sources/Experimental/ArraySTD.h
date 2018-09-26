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
#include <vector>

namespace cross{

template<class T>
class ArraySTD : public std::vector<T> {
public:
	ArraySTD();
	ArraySTD(U32 capacity);
	ArraySTD(U32 capacity, const T& init);
	ArraySTD(std::initializer_list<T> list);

	void Add(const T& item);
	S32 Size() const;
	S32 Capacity() const;
};

template<class T>
ArraySTD<T>::ArraySTD() :
	std::vector<T>()
{ }

template<class T>
ArraySTD<T>::ArraySTD(U32 capacity) :
	std::vector<T>(capacity)
{ }

template<class T>
ArraySTD<T>::ArraySTD(U32 capacity, const T& init) :
	std::vector<T>(capacity, init)
{ }

template<class T>
ArraySTD<T>::ArraySTD(std::initializer_list<T> list) :
	std::vector<T>(list)
{ }

template<class T>
void ArraySTD<T>::Add(const T& item) {
	push_back(item);
}

template<class T>
S32 ArraySTD<T>::Size() const {
	return size();
}

template<class T>
S32 ArraySTD<T>::Capacity() const {
	return capacity();
}

}