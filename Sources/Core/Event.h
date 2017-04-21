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
    typedef uint64_t U64;

    void Emit(Args... p);

    U64 Connect(std::function<void(Args...)> const& func);
    template<class Clazz>
    U64 Connect(Clazz* obj, void(Clazz::*func)(Args...));
    template<class Clazz>
    U64 Connect(Clazz* obj, void(Clazz::*func)(Args...) const);

    void Disconnect(U64 del);
    void DisconnectAll();

    void operator () (Args...);

private:
    std::map<U64, std::function<void(Args...)>> connections;
    long connectID = 0;
};

//implementation
template<class... Args>
void Event<Args...>::Emit(Args... p){
    for (auto it : connections) {
        it.second(p...);
    }
}

template<class... Args>
U64 Event<Args...>::Connect(std::function<void(Args...)> const& func){
    connections.insert(std::make_pair(connectID, func));
    return connectID++;
}

template<class... Args>
template<class Clazz>
U64 Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...)){
    return Connect([=](Args... args){
        (obj->*func)(args...);
    });
}

template<class... Args>
template<class Clazz>
U64 Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...) const) {
    return Connect([=](Args... args) {
        (obj->*func)(args...);
    });
}

template<class... Args>
void Event<Args...>::Disconnect(U64 del) {
    connections.erase(del);
}


template<class... Args>
void Event<Args...>::DisconnectAll(){
    connections.clear();
}

template<class... Args>
void Event<Args...>::operator () (Args... args){
    Emit(args...);
}

};