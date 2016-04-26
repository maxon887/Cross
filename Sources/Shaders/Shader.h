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
#include "GraphicsGL.h"

namespace cross{

class Shader{
public:
	enum Type{
		SIMPLE,
		MONOCHROME,
		TEXTURE,
		LIGHT_CASTER_MATERIAL,
		LIGHT_CASTER_DIFFUSE,
		LIGHT_CASTER_DIFFUSE_SPECULAR,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		COUNT
	};

	GLuint program;
	GLint uMVP;
	GLint aPosition;
	Type type;

	Shader(Type type, const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

private:
	GLuint vertex_shader;
	GLuint fragment_shader;
};

}