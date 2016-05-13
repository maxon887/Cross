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
		LIGHT_MATERIAL,
		LIGHT_DIFFUSE,
		LIGHT_DIFFUSE_SPECULAR,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		MULTI_LIGHT,
		COUNT
	};

	static const int MaxDiffuseMaps = 8;
	static const int MaxSpecularMaps = 8;

	GLuint program;
	GLint uMVP;
	//shader attributes
	GLint aPosition;
	GLint aTexCoords;
	GLint aNormal;
	GLint uDiffuseMaps[MaxDiffuseMaps];
	GLint uSpecularMaps[MaxSpecularMaps];

	Type type;

	Shader(Type type, const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

private:
	GLuint vertex_shader;
	GLuint fragment_shader;
};

}