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
	Function(Lambda& lamb) {
		this->lambda = new Lambda(lamb);
		//this->lambda = &lamb;

		executer = [](void* l, Input... args) -> Ret {
			return ((Lambda*)l)->operator()(args...);
		};
	};

	template<class Clazz>
	Function(Clazz* obj, Ret(Clazz::*meth)(Input... args)) {
		method = new Function([obj, meth]() {
			(obj->*meth)(args...);
		});
	}

	Function(const Function& other) {

	}

	~Function() {
		delete lambda;
		delete method;
	}

	Ret operator ()(Input... args) {
		if(method) {
			return (*method)(args...);
		} else if(lambda) {
			executer(lambda, args...);
		}
	}

private:
	void* lambda = nullptr;
	void(*copy)(void*);
	Ret(*executer)(void*, Input...);
	Function* method = nullptr;
};