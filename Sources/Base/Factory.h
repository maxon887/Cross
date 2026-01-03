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
#include "Base/Function.h"

#include <typeinfo>

namespace cross{

/*	Generic Factory template. This will allow you to create successor classes by name.
	This is useful when you are interested in particular class type but can not create it by your self.
	f.e. engine Component system can create user component, but it doesn't know about your components by default.
	Because of that you need to register your components before do that. Or use this class in your personal goals*/
template<class BaseClass>
class Factory {
public:
	/* Registers Component Type. Provide a name by which Component will be loaded from XML file (and in other visual representations) */
	template<class AdvancedClass> void Register(const String& name);
	/* Creates successor by name of Component of one of registered classes */
	BaseClass* Create(const String& name);
	/* Obtains all registered Components types */
	Array<String> GetRegisteredComponentsName();
	/* Return name associated with class of the provide object */
	String GetNameByClass(BaseClass* object);

private:
	Map<String, Function<BaseClass*()>> functions;
	Array<String> names;
	Map<S64, String> class_hashes;
};

template<class BaseClass>
template<class AdvancedClass>
void Factory<BaseClass>::Register(const String& name) {
	functions.insert(std::make_pair(name, []() -> BaseClass* { return CREATE AdvancedClass(); }));
	names.Add(name);
	class_hashes.insert(std::make_pair(typeid(AdvancedClass).hash_code(), name));
}

template<class BaseClass>
BaseClass* Factory<BaseClass>::Create(const String& name) {
	CROSS_RETURN(functions.find(name) != functions.end(), nullptr, "AdvancedClass # didn't register in the factory", name);
	return functions[name]();
}

template<class BaseClass>
Array<String> Factory<BaseClass>::GetRegisteredComponentsName() {
	return names;
}

template<class BaseClass>
String Factory<BaseClass>::GetNameByClass(BaseClass* object) {
	auto found = class_hashes.find(typeid(*object).hash_code());
	if(found != class_hashes.end()) {
		return found->second;
	}
	CROSS_RETURN(false, "", "Can not find class name by object");
}

}