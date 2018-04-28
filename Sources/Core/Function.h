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

/*	Function class needed for lazy binding. For example you can bind a function or method 
	to this class object and call it later. */
template<class Ret, class... Input>
class Function<Ret(Input...)> {
public:
	template<class Lambda>	Function(const Lambda& lambda);
	template<class Class>	Function(Class* obj, Ret(Class::*meth)(Input... args));
							Function(const Function& other);
							~Function();

	Ret operator ()(Input... args);

private:
	void* lambda = nullptr;
	Ret(*executer)(void*, Input...);
	void*(*copier)(void*);

	template<class Lambda>
	void Init(const Lambda& other);
};

template<class Ret, class... Input>
template<class Lambda>
Function<Ret(Input...)>::Function(const Lambda& lambda) {
	Init(lambda);
}

template<class Ret, class... Input>
template<class Class>
Function<Ret(Input...)>::Function(Class* obj, Ret(Class::*meth)(Input... args)) {
	Init([obj, meth](Input... args) {
		(obj->*meth)(args...);
	});
}

template<class Ret, class... Input>
Function<Ret(Input...)>::Function(const Function& other) {
	lambda = other.copier(other.lambda);
	executer = other.executer;
	copier = other.copier;
}

template<class Ret, class... Input>
Function<Ret(Input...)>::~Function() {
	delete lambda;
}

template<class Ret, class... Input>
Ret Function<Ret(Input...)>::operator()(Input... args) {
	assert(lambda);
	executer(lambda, args...);
}

template<class Ret, class... Input>
template<class Lambda>
void Function<Ret(Input...)>::Init(const Lambda& other) {
	lambda = new Lambda(other);

	executer = [](void* lamb, Input... args) -> Ret {
		return ((Lambda*)lamb)->operator()(args...);
	};

	copier = [](void* source) -> void* {
		return new Lambda(*(Lambda*)source);
	};
}