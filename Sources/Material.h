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
#include "Shaders/Shader.h"

namespace cross{

class Material{
public:
	Array<Shader::Property*> properties;
	U32 active_texture_slot;

	Material(Shader* shader);
	~Material();

	void SetShader(Shader* shader);
	Shader::Property* GetProperty(const string& name);
	Shader::Property* GetProperty(GLuint glID);

	void SetPropertyValue(const string& name, U32 value);
	void SetPropertyValue(const string& name, float value);
	void SetPropertyValue(const string& name, const Color& value);
	void SetPropertyValue(const string& name, Vector3D& value);
	void SetPropertyValue(const string& name, Vector4D& value);
	void SetPropertyValue(const string& name, Matrix& value);
	void SetPropertyValue(const string& name, Texture* value);
	void SetPropertyValue(GLuint glID, U32 value);
	void SetPropertyValue(GLuint glID, float value);
	void SetPropertyValue(GLuint glID, const Color& value);
	void SetPropertyValue(GLuint glID, Vector3D& value);
	void SetPropertyValue(GLuint glID, Vector4D& value);
	void SetPropertyValue(GLuint glID, Matrix& value);
	void SetPropertyValue(GLuint glID, Texture* value);

	Shader* GetShader();

	Material* Clone();

private:
	Shader* shader;

	Material(Material& obj);
};

}