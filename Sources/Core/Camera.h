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
#include "Component.h"

namespace cross {

/*	3D Camera Component. At least one camera must be set into Scene by SetCamera() in order to render a Scene.
	Camera can't be used until SetProjectionMatrix() is called.
	Camera Component can't be used with Entity without Transform Component */
class Camera : public Component {
public:
	Camera();
	/* Updates Camera Component. Realized in order to recalculate view Matrix */
	void Update(float sec) override;
	/* Save copy of Camera Component */
	Component* Clone() const override;
	/* Loads Camera Component from XML document */
	bool Load(tinyxml2::XMLElement* xml, Scene* loadingScene) override;

	/* Returns Camera view distance. Object under that distance won't be rendered */
	float GetViewDistance() const;
	/* Returns Camera's view Matrix. Depends on Entity's Transform Component */
	const Matrix& GetViewMatrix() const;
	/* Returns Camera's projection Matrix. Must be setup first */
	const Matrix& GetProjectionMatrix() const;
	/* Sets Camera's projection Matrix. Needed to render a Scene */
	void SetProjectionMatrix(const Matrix& projection);

protected:
	Property<float> view_distance = Property<float>(this, "ViewDistance", 100.f);

	Matrix view						= Matrix::Identity;
	Matrix projection				= Matrix::Identity;

	void RecalcView();
};

};