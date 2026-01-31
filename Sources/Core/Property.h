#pragma once
#include "Cross.h"
#include "System.h"
#include "Color.h"
#include "Math/All.h"

#include "Libs/TinyXML2/tinyxml2.h"

namespace cross{

class BaseProperty {
public:
	BaseProperty(Component* owner, String name);

	virtual bool Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const = 0;
	virtual bool Load(tinyxml2::XMLElement* parent) = 0;
	virtual bool IsEnum() const = 0;
	virtual String ToString() const = 0;

	String GetName() const;

protected:
	String name;
};

/* Properties made for Scene loading mechanism. They allow Components to have variable fields
 which will later be saved in a file. They also useful for viewing properties in UI */
template<class T>
class Property : public BaseProperty {
public:
	Event<> ValueChanged;

	Property(Component* owner, String name);
	Property(Component* owner, String name, const T& def);

	const T& Get() const;

	bool Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const override;
	bool Load(tinyxml2::XMLElement* parent) override;
	bool IsEnum() const override;
	String ToString() const override;

	T& operator = (const T& val);
	T& operator = (const Property& other);
	operator const T& () const;

protected:
	T value;
	bool is_required = true;
};

template<class T>
Property<T>::Property(Component* owner, String name) :
	BaseProperty(owner, std::move(name))
{ }

template<class T>
Property<T>::Property(Component* owner, String name, const T& def) :
	BaseProperty(owner, std::move(name)), value(def), is_required(false)
{ }

template<class T>
const T& Property<T>::Get() const {
	return value;
}

template<class T>
bool Property<T>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
	using namespace tinyxml2;
	constexpr bool isEnum = std::is_enum<T>::value;
	if(isEnum) {
		XMLElement* propertyXML = doc->NewElement(name);
		propertyXML->SetAttribute("type", "Enum");
		propertyXML->SetAttribute("value", value);
		parent->LinkEndChild(propertyXML);
		return true;
	} else {
		CROSS_RETURN(false, false, "Unknown property type to save (#)", name);
	}
}

template<class T>
inline bool Property<T>::IsEnum() const {
	constexpr bool isEnum = std::is_enum<T>::value;
	return isEnum;
}

template<class T>
inline String Property<T>::ToString() const {
	CROSS_RETURN(false, false, "Called unspecialized function ToString() for property (#)", name);
}

template<>
inline bool Property<S32>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Int");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<float>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Float");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<String>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
	using namespace tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "String");
	propertyXML->SetAttribute("value", value);
	parent->LinkEndChild(propertyXML);
	return true;
}

template<>
inline bool Property<Vector3D>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
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
inline bool Property<Quaternion>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
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

template<>
inline bool Property<Color>::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) const {
	using namespace  tinyxml2;
	XMLElement* propertyXML = doc->NewElement(name);
	propertyXML->SetAttribute("type", "Color");
	propertyXML->SetAttribute("data", value.ToString());
	parent->LinkEndChild(propertyXML);
	return true;
}

template<class T>
bool Property<T>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	constexpr bool isEnum = std::is_enum<T>::value;
	if(isEnum) {
		XMLElement* propertyXML = parent->FirstChildElement(name);
		if(!propertyXML) {
			CROSS_RETURN(is_required, false, "Required Property '#' not found", name);
			return true;
		}
		String typeStr = propertyXML->Attribute("type");
		CROSS_RETURN(typeStr == "Enum", false, "Loading attribute mismatch. Expected Enum");
		value = (T)propertyXML->Int64Attribute("value");
		return true;
	} else {
		CROSS_RETURN(false, false, "Unknown property type to load(#)", name);
	}
}

template<>
inline bool Property<S32>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Int", false, "Loading attribute mismatch. Expected Int");
	value = (S32)propertyXML->Int64Attribute("value");
	return true;
}

template<>
inline bool Property<float>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(!is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Float", false, "Loading attribute mismatch. Expected Float");
	value = propertyXML->FloatAttribute("value");
	return true;
}

template<>
inline bool Property<String>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(!is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "String", false, "Loading attribute mismatch. Expected String");
	value = propertyXML->Attribute("value");
	return true;
}

template<>
inline bool Property<Vector3D>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(!is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Vector3D", false, "Loading attribute mismatch. Expected Vector3D");
	value.x = propertyXML->FloatAttribute("x");
	value.y = propertyXML->FloatAttribute("y");
	value.z = propertyXML->FloatAttribute("z");
	return true;
}

template<>
inline bool Property<Quaternion>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(!is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Quaternion", false, "Loading attribute mismatch. Expected Quaternion");
	value.x = propertyXML->FloatAttribute("x");
	value.y = propertyXML->FloatAttribute("y");
	value.z = propertyXML->FloatAttribute("z");
	value.w = propertyXML->FloatAttribute("w");
	return true;
}

template<>
inline bool Property<Color>::Load(tinyxml2::XMLElement* parent) {
	using namespace tinyxml2;
	XMLElement* propertyXML = parent->FirstChildElement(name);
	if(!propertyXML) {
		CROSS_RETURN(!is_required, false, "Required Property '#' not found", name);
		return true;
	}
	String typeStr = propertyXML->Attribute("type");
	CROSS_RETURN(typeStr == "Color", false, "Loading attribute mismatch. Expected Color");
	value = propertyXML->Attribute("data");
	return true;
}

template<class T>
T& Property<T>::operator = (const T& val) {
	value = val;
	ValueChanged.Emit();
	return value;
}

template<class T>
T& Property<T>::operator = (const Property<T>& other) {
	value = other.Get();
	ValueChanged.Emit();
	return value;
}

template<class T>
Property<T>::operator const T& () const {
	return value;
}

}
