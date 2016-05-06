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
#include "Shaders/Shader.h"

namespace cross{

class MultiLightShader : public Shader{
public:
	struct PointLightLoc{
		GLuint position;
	
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	
		GLuint constant;
		GLuint linear;
		GLuint quadratic;
	};

	struct DirectionalLight{
		GLuint direction;
	
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	};

	struct SpotLight{
		GLuint position;
	
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	
		GLuint constant;
		GLuint linear;
		GLuint quadratic;
	
		GLuint direction;
		GLuint cut_off;
		GLuint outer_cut_off;
	};

	static const unsigned int MaxPointLights = 32;
	static const unsigned int MaxDirectionalLights = 8;
	static const unsigned int MaxSpotLights = 16;

	GLuint uModelMatrix;
	GLuint uNormalMatrix;

	GLuint uMaterialDiffuse;
	GLuint uMaterialSpecular;
	GLuint uMaterialShininess;

	GLuint uPointLightCount;
	PointLightLoc uPointLights[MaxPointLights];

	GLuint uDirectionalLightCount;
	DirectionalLight uDirectionalLights[MaxDirectionalLights];

	GLuint uSpotLightCount;
	SpotLight uSpotLights[MaxSpotLights];

	GLuint uCameraPosition;

	GLuint aTexCoords;
	GLuint aNormal;

	MultiLightShader();
};

}