#include "Component.h"
#include "Entity.h"
#include "Transform.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Component::Component(String name) :
	name(std::move(name))
{ }

Component* Component::Clone() const {
	CROSS_RETURN(false, nullptr, "Can't clone component. Inherited class doesn't implement Clone() function");
}

bool Component::Load(tinyxml2::XMLElement* parent) {
	enabled = parent->BoolAttribute("Enabled", true);
	bool success = true;
	for(BaseProperty* prop : properties) {
		bool propertyLoaded = prop->Load(parent);
		CROSS_ASSERT(propertyLoaded, "Component '#' have problem during loading", GetName());
		if(!propertyLoaded) {
			success = false;
		}
	}
	return success;
}

bool Component::Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc) {
	XMLElement* componentXML = doc->NewElement(name);
	componentXML->SetAttribute("Enabled", enabled);
	for(BaseProperty* prop : properties) {
		CROSS_RETURN(prop->Save(componentXML, doc), false, "Can not save component '#'", GetName());
	}
	parent->LinkEndChild(componentXML);
	return true;
}

bool Component::IsEnabled() const {
	return enabled;
}

void Component::Enable() {
	enabled = true;
	SetActive();
}

void Component::Disable() {
	enabled = false;
	Deactivate();
}

void Component::SetActive() {
	if(!enabled) {
		return;
	}
	CROSS_FAIL(entity && entity->IsOnScene(), "Trying to Enable Component without parent on Scene");
	if(!initialized) {
		initialized = Initialize();
		if(!initialized) {
			enabled = false;
			CROSS_FAIL(false, "Can not Initialize Component '#'", GetName());
		}
	}
	enabled = Activate();
	CROSS_ASSERT(enabled, "Can not Activate Component '#'", GetName())
}

void Component::SetInactive() {
	Deactivate();
}

String Component::GetName() const {
	return name;
}

Entity* Component::GetEntity() {
	return entity;
}

const Array<BaseProperty*>& Component::GetProperties() const {
	return properties;
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