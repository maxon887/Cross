/*	Copyright © 2015 Lukyanau Maksim

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

namespace cross{

class Light{
public:

	Light(const Vector3D& ambient, const Vector3D& diffuse, const Vector3D& specular);
	virtual ~Light();

	void SetAmbientStrength(const Vector3D& ambient);
	void SetDiffuseStrength(const Vector3D& diffuse);
	void SetSpecularStrength(const Vector3D& specular);

	Vector3D GetAmbientStrength();
	Vector3D GetDiffuseStrength();
	Vector3D GetSpecularStrength();
protected:
	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
};

}