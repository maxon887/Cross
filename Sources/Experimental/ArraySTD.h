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

template<class Type>
class ArraySTD : public std::vector<Type> {
public:
	ArraySTD();
	ArraySTD(U32 capacity);
	ArraySTD(U32 capacity, const Type& init);
	ArraySTD(std::initializer_list<Type> list);
};

template<class Type>
ArraySTD<Type>::ArraySTD() :
	std::vector<Type>()
{ }

template<class Type>
ArraySTD<Type>::ArraySTD(U32 capacity) :
	std::vector<Type>(capacity)
{ }

template<class Type>
ArraySTD<Type>::ArraySTD(U32 capacity, const Type& init) :
	std::vector<Type>(capacity, init)
{ }

template<class Type>
ArraySTD<Type>::ArraySTD(std::initializer_list<Type> list) :
	std::vector<Type>(list)
{ }

}