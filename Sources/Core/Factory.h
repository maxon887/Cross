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
#include "Experimental/Function.h"

namespace cross{

/*	Generic Factory template. This will allow you to create successor classes by name.
	This is usefull when you interested particyla class type but can not create it by your self.
	f.e. engine Component system can create user component but it doesn't know about your components by default.
	Because of that you need to register your components before do that. Or use this class in personal goals*/
template<class BaseClass>
class Factory {
public:
	/* Registers Component Type. Provide a name by which Component will be loaded from XML file (and in other visual representations) */
	template<class AdvancedClass> void Register(const String& name);
	/* Creates successor by name of Component of one of registered classes */
	BaseClass* Create(const String& name);
	/* Obtains all registered Components types */
	Array<String> GetRegisteredComponentsName();

private:
	Dictionary<String, Function<BaseClass*()>> functions;
};

template<class BaseClass>
template<class AdvancedClass>
void Factory<BaseClass>::Register(const String& name) {
	functions.insert(std::make_pair(name, []() -> BaseClass* { return new AdvancedClass(); }));
}

template<class BaseClass>
BaseClass* Factory<BaseClass>::Create(const String& name) {
	CROSS_RETURN(functions.find(name) != functions.end(), nullptr, "AdvancedClass # didn't register in the factory", name);
	return functions[name]();
}

template<class BaseClass>
Array<String> Factory<BaseClass>::GetRegisteredComponentsName() {
	Array<String> names;
	for(std::pair<String, Function<Component* ()>> pair : functions) {
		names.Add(pair.first);
	}
	return names;
}

};