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
#include "Cross.h"

#include <functional>

namespace cross{

class Component;

class ComponentFactory {
public:
	template<class T>
	void Register(const string& name) {
		functions.insert(std::make_pair(name, []() -> Component* { return new T(); }));
	}

	Component* Create(const string& name) {
		CROSS_RETURN(functions.find(name) != functions.end(), NULL, "Component %s didn't register in the factory", name.c_str());
		return functions[name]();
	}

private:
	Dictionary<string, std::function<Component*()>> functions;
};

}