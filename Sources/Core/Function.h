/*	Copyright © 2018 Maksim Lukyanov

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

template<class T>
class Function;

template<class Ret, class... Input>
class Function<Ret(Input...)> {
public:

	template<class Lambda>
	Function(Lambda& other) {
		Init(other);
	};

	template<class Class>
	Function(Class* obj, Ret(Class::*meth)(Input... args)) {
		Init([obj, meth]() {
			(obj->*meth)(args...);
		});
	}

	~Function() {
		delete lambda;
	}

	Ret operator ()(Input... args) {
		assert(lambda);
		executer(lambda, args...);
	}

private:
	void* lambda = nullptr;
	Ret(*executer)(void*, Input...);

	template<class Lambda>
	void Init(Lambda& other) {
		lambda = new Lambda(other);

		executer = [](void* lamb, Input... args) -> Ret {
			return ((Lambda*)lamb)->operator()(args...);
		};
	}
};