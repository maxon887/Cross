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
#include <functional>

namespace cross{

template<class... Args>
class Event{
public:
    void Emit(Args... p);

    void Connect(std::function<void(Args...)> const& func);
    template<class Clazz>
    void Connect(Clazz* obj, void(Clazz::*func)(Args...));
    template<class Clazz>
    void Connect(Clazz* obj, void(Clazz::*func)(Args...) const);

    void Disconnect(std::function<void(Args...)> const& func);
    template<class Clazz>
    void Disconnect(Clazz* obj, void(Clazz::*func)(Args...));
    template<class Clazz>
    void Disconnect(Clazz* obj, void(Clazz::*func)(Args...) const);

    void DisconnectAll();

    void operator () (Args...);

private:
    List<std::function<void(Args...)>> listeners;
};

//implementation
template<class... Args>
void Event<Args...>::Emit(Args... p){
    for (auto it : listeners) {
        it(p...);
    }
}

template<class... Args>
void Event<Args...>::Connect(std::function<void(Args...)> const& func){
    listeners.push_back(func);
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...)){
    Connect([=](Args... args){
        (obj->*func)(args...);
    });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...) const) {
    Connect([=](Args... args) {
        (obj->*func)(args...);
    });
}

template<class... Args>
void Event<Args...>::Disconnect(std::function<void(Args...)> const& func) {
    for (auto it = listeners.begin(); it != listeners.end(); it++) {
        if (*it = func) {
            listeners.erase(it++);
            break;
        }
    }
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Disconnect(Clazz* obj, void(Clazz::*func)(Args...)) {
    Disconnect([=](Args... args) {
        (obj->*func)(args...);
    });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Disconnect(Clazz* obj, void(Clazz::*func)(Args...) const) {
    Disconnect([=](Args... args) {
        (obj->*func)(args...);
    });
}


template<class... Args>
void Event<Args...>::DisconnectAll(){
    listeners.clear();
}

template<class... Args>
void Event<Args...>::operator () (Args... args){
    Emit(args...);
}

};