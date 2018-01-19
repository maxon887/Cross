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
#include "Cross.h"

#include <functional>

namespace cross{

class Component;

/*	This class needed for custom Component registration. 
	All Component types must be register in order to support loading from scene XML files.
	Register your Components before any Scene::Load() calls */
class ComponentFactory {
public:
	/* Registers Component Type. Provide a name by which Component will be loaded from XML file (and visually presented in some causes) */
	template<class T> void Register(const string& name);
	/* Creates successor by name of Component of one of registered classes */
	Component* Create(const string& name);
	/* Obtains all registered Components types */
	Array<string> GetRegisteredComponentsName();

private:
	Dictionary<string, std::function<Component*()>> functions;
};

template<class T>
void ComponentFactory::Register(const string& name) {
	functions.insert(std::make_pair(name, []() -> Component* { return new T(); }));
}

};