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
#include "Component.h"
#include "System.h"
#include "Entity.h"
#include "Transform.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Component::Component(const String& name) :
	name(name)
{ }

Component* Component::Clone() const {
	CROSS_RETURN(false, nullptr, "Can't clone component. Inherited class doesn't implement Clone() function");
}

bool Component::Load(tinyxml2::XMLElement*, Scene*) {
	CROSS_RETURN(false, false, "Can't load component. Inherited class doesn't implement Load() function");
}

bool Component::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	XMLElement* componentXML = doc->NewElement(name);
	for(BaseProperty* prop : properties) {
		prop->Save(componentXML, doc);
	}
	parent->LinkEndChild(componentXML);
	return true;
}

bool Component::IsEnabled() const {
	return enabled;
}

void Component::Enable(bool e) {
	this->enabled = e;
}

Entity* Component::GetEntity() {
	return entity;
}

Transform* Component::GetTransform() {
	return entity->GetTransform();
}

Vector3D Component::GetPosition() const {
	return entity->GetTransform()->GetPosition();
}

void Component::SetPosition(const Vector3D& pos) {
	entity->GetTransform()->SetPosition(pos);
}