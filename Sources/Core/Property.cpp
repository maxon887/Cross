#include "Property.h"
#include "Component.h"

using namespace cross;

BaseProperty::BaseProperty(Component* owner, String name) :
	name(std::move(name))
{
	owner->properties.Add(this);
}

String BaseProperty::GetName() const {
	return name;
}