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

	struct PointLight{
		GLint position;
		GLint color;

		GLint linear;
		GLint quadratic;
	};

	struct DirectionalLight{
		GLint direction;
		GLint color;
	};

	struct SpotLight{
		GLint position;
		GLuint direction;
		GLint color;

		GLint linear;
		GLint quadratic;
	
		GLuint cut_off;
		GLuint outer_cut_off;
	};

	static const unsigned int MaxPointLights = 16;
	static const unsigned int MaxDirectionalLights = 4;
	static const unsigned int MaxSpotLights = 8;
	
	GLuint uMaterialDiffuse;
	GLuint uMaterialSpecular;
	GLuint uMaterialShininess;

	GLuint uPointLightCount;
	PointLight uPointLights[MaxPointLights];

	GLuint uDirectionalLightCount;
	DirectionalLight uDirectionalLights[MaxDirectionalLights];

	GLuint uSpotLightCount;
	SpotLight uSpotLights[MaxSpotLights];

	MultiLightShader();

	virtual bool UseLights();
	virtual void TransferLightData(const CRArray<Light*>& lights);
};

}