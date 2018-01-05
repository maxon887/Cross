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
#include <functional>

namespace cross{

template<class... Args>
class Event {
public:
	void Emit(Args... p);

	void Connect(U64 hash, std::function<void(Args...)> const& func);
	template<class Clazz>
	void Connect(Clazz* obj, void(Clazz::*func)(Args...));
	template<class Clazz>
	void Connect(Clazz* obj, void(Clazz::*func)(Args...) const);
	template<class Clazz>
	void Disconnect(Clazz* obj, void(Clazz::*func)(Args...));
	void DisconnectAll();

	void operator () (Args...);

private:
	std::map<U64, std::function<void(Args...)>> connections;

	template<class Clazz>
	U64 Hash(Clazz* obj, void(Clazz::*func)(Args...));
};

//implementation
template<class... Args>
void Event<Args...>::Emit(Args... p) {
	for (auto it : connections) {
		it.second(p...);
	}
}

template<class... Args>
void Event<Args...>::Connect(U64 hash, std::function<void(Args...)> const& func) {
	connections.insert(std::make_pair(hash, func));
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...)) {
	return Connect(Hash(obj, func), [=](Args... args) { (obj->*func)(args...); });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...) const) {
	return Connect(Hash(obj, func), [=](Args... args) { (obj->*func)(args...); });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Disconnect(Clazz* obj, void(Clazz::*func)(Args...)) {
	U64 hash = Hash(obj, func);
	CROSS_FAIL(connections.find(hash) != connections.end(), "Function does not connected");
	connections.erase(hash);
}

template<class... Args>
void Event<Args...>::DisconnectAll() {
	connections.clear();
}

template<class... Args>
void Event<Args...>::operator () (Args... args) {
	Emit(args...);
}

template<class... Args>
template<class Clazz>
U64 Event<Args...>::Hash(Clazz* obj, void(Clazz::*func)(Args...)) {
	void* fPointer = (void*&)func;
	return (U64)obj + (U64)fPointer;
}

};