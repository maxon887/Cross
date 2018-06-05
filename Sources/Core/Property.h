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
#include "System.h"

#include "Libs/TinyXML2/tinyxml2.h"

namespace cross{

class BaseProperty {
public:
	virtual bool Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) = 0;
	virtual bool Load(tinyxml2::XMLElement* parent) = 0;

protected:
	void Init(Component* comp);
};

template<class T>
class Property : public BaseProperty {
public:
	T value;
	String name;

	Property(Component* comp, const String& name);
	Property(Component* comp, const String& name, const T& def);

	bool Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) override;
	bool Load(tinyxml2::XMLElement* parent) override;

	T& operator = (const T& val);
	operator T () const;
};

template<class T>
Property<T>::Property(Component* comp, const String& name) {
	this->name = name;
	Init(comp);
}

template<class T>
Property<T>::Property(Component* comp, const String& name, const T& def) {
	this->value = def;
	this->name = name;
	Init(comp);
}

template<class T>
bool Property<T>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	CROSS_RETURN(false, false, "Unknown property type to save (#)", name);
}

template<>
inline bool Property<S32>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Int");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<float>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Float");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<String>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "String");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<Vector3D>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Vector3D");
	propertyXML->SetAttribute("x", value.x);
	propertyXML->SetAttribute("y", value.y);
	propertyXML->SetAttribute("z", value.z);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<Quaternion>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Quaternion");
	propertyXML->SetAttribute("x", value.x);
	propertyXML->SetAttribute("y", value.y);
	propertyXML->SetAttribute("z", value.z);
	propertyXML->SetAttribute("w", value.w);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<class T>
bool Property<T>::Load(tinyxml2::XMLElement* parent) {
	CROSS_RETURN(false, false, "Unknown property type to load(#)", name);
}

template<>
inline bool Property<S32>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Int", false, "Loading attribute missmatch. Expected Int");
	value = (S32)propertyXML->Int64Attribute("value");
	return true;
}

template<>
inline bool Property<float>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Float", false, "Loading attribute missmatch. Expected Float");
	value = propertyXML->FloatAttribute("value");
	return true;
}

template<>
inline bool Property<String>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "String", false, "Loading attribute missmatch. Expected String");
	value = propertyXML->Attribute("value");
	return true;
}

template<>
inline bool Property<Vector3D>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Vector3D", false, "Loading attribute missmatch. Expected Vector3D");
	value.x = propertyXML->FloatAttribute("x");
	value.y = propertyXML->FloatAttribute("y");
	value.z = propertyXML->FloatAttribute("z");
	return true;
}

template<>
inline bool Property<Quaternion>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Quaternion", false, "Loading attribute missmatch. Expected Quaternion");
	value.x = propertyXML->FloatAttribute("x");
	value.y = propertyXML->FloatAttribute("y");
	value.z = propertyXML->FloatAttribute("z");
	value.w = propertyXML->FloatAttribute("w");
	return true;
}

template<class T>
T& Property<T>::operator = (const T& val) {
	value = val;
	return value;
}

template<class T>
Property<T>::operator T () const {
	return value;
}

}
