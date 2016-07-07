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
#include "Shader.h"

namespace cross{

class Material{
public:
	Material(const string& name);
	Material(Shader* shader);
	~Material();

	void SetShader(Shader* shader);
	void SetDiffuseColor(const Color& color);
	void SetSpecularColor(const Color& color);
	void SetShininess(float shininess);
	void SetDiffuseTexture(Texture* texture);

	Color GetDiffuseColor();
	Color GetSpecularColor();
	float GetShininess();
	Texture* GetDiffuseTexture();

	bool HasDiffuseTexture();

	Shader* GetShader();
private:
	string name;
	Shader* shader;
	Color diffuse_color;
	Color specular_color;
	float shininess;
	Texture* diffuse_texture;
};

}