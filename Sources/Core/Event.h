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

namespace cross{

/*	Event provides functional for object to notify other instances when something of interest happened.
	Class that want to notify of something must call Emit() function of Event the rest object which was
	interested in particular Event must subscribed for this event by calling Connect() function */
template<class... Args>
class Event {
public:
	/* Notifies all subscribers than something of interest has happened by calling their functions provided in Connect() function */
	void Emit(Args... args);

	/* Subscribes current object on this Event. Object must provide its class function which will be called when Event Occurs */
	template<class Class> void Connect(Class* obj, void(Class::*func)(Args...));
	/* Subscribes current object on this Event. Object must provide its const class function which will be called when Event Occurs */
	template<class Class> void Connect(Class* obj, void(Class::*func)(Args...) const);
	/* Unsubscribes current object from this Event. IMPORTANT! All objects must be disconnected from Events before they die */
	template<class Class> void Disconnect(Class* obj, void(Class::*func)(Args...));
	/* Disconnect all subscribers from this Event */
	void DisconnectAll();

private:
	std::map<U64, Function<void(Args...)>> connections;

	template<class Clazz> U64 Hash(Clazz* obj, void(Clazz::*func)(Args...));
	void Connect(U64 hash, const Function<void(Args...)>& func);
};

//implementation
template<class... Args>
void Event<Args...>::Emit(Args... args) {
	for (auto it : connections) {
		it.second(args...);
	}
}

template<class... Args>
template<class Class>
void Event<Args...>::Connect(Class* obj, void(Class::*func)(Args...)) {
	Connect(Hash(obj, func), Function<void(Args...)>(obj, func));
}

template<class... Args>
template<class Class>
void Event<Args...>::Connect(Class* obj, void(Class::*func)(Args...) const) {
	Connect(Hash(obj, func), Function<void(Args...)>(obj, func));
}

template<class... Args>
template<class Class>
void Event<Args...>::Disconnect(Class* obj, void(Class::*func)(Args...)) {
	U64 hash = Hash(obj, func);
	//Function does not connected
	if(connections.find(hash) == connections.end()) {
		assert(false);
	}
	connections.erase(hash);
}

template<class... Args>
void Event<Args...>::DisconnectAll() {
	connections.clear();
}

template<class... Args>
template<class Class>
U64 Event<Args...>::Hash(Class* obj, void(Class::*func)(Args...)) {
	void* fPointer = (void*&)func;
	return (U64)obj + (U64)fPointer;
}

template<class... Args>
void Event<Args...>::Connect(U64 hash, const Function<void(Args...)>& func) {
	connections.insert(std::make_pair(hash, func));
}

};
