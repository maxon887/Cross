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
#include "Property.h"

namespace tinyxml2 {
	class XMLElement;
	class XMLDocument;
}

namespace cross{

/*	Components used for create Entity special behavior. For example components can be renderable, physic behavior, sound etc.
	User components must be register by ComponentFactory. Components have save life time as an Entity */
class Component {
public:
	Component(const String& name);
	virtual ~Component() { }

	/* Will be called on component after component was added to an Entity */
	virtual void Initialize() { }
	/* Will be called after component was removed from Entity or on the death of Entity*/
	virtual void Remove() { }
	/* Will be called every game cycle. WARNING! Components update order unpredictable */
	virtual void Update(float sec) { }

	/* Clone whole component. Must be implemented in order to support Entity copy and spawn operations */
	virtual Component* Clone() const;
	/* Load Component from XML document. Must be implemented to support Component loading from Scene file */
	virtual bool Load(tinyxml2::XMLElement* parent, Scene* loadingScene);
	/* Save Component into XML document. Must be implemented to support Component save to Scene file */
	virtual bool Save(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc);

	/* Returns true if Component behavior is enabled */
	bool IsEnabled() const;
	/* Enables or disables Component behavior */
	void Enable(bool enable);

	/* Returns Entity that owns this Component */
	Entity* GetEntity();
	/* Returns Entity's Transform Component if has some */
	Transform* GetTransform();
	/* Returns Entity's Transform's position vector */
	Vector3D GetPosition() const;
	/* Set position into into Entity's Transform Component */
	void SetPosition(const Vector3D& pos);

private:
	friend Entity;
	friend BaseProperty;

	String name		= "noname";
	Entity* entity	= nullptr;
	bool enabled	= true;
	Array<BaseProperty*> properties;
};

}