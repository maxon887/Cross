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
#include "ComponentFactory.h"
#include "System.h"

using namespace cross;

Component* ComponentFactory::Create(const String& name) {
	CROSS_RETURN(functions.find(name) != functions.end(), nullptr, "Component # didn't register in the factory", name);
	return functions[name]();
}

Array<String> ComponentFactory::GetRegisteredComponentsName() {
	Array<String> names;
	for(std::pair<String, Function<Component*()>> pair : functions) {
		names.Add(pair.first);
	}
	return names;
}