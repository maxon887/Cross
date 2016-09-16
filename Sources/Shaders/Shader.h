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
	class Property{
	public:
		enum Type{
			SAMPLER,
			MAT4,
			MAT3,
			VEC4,
			VEC3,
			VEC2,
			FLOAT,
			INT
		};

		Property(string name, Type type, string glName):
			name(name),
			type(type),
			glName(glName),
			glId(-1)
		{ }

		string name;
		Type type;
		string glName;
		GLint glId;
	};
	//general attributes
	GLint aPosition;
	GLint aTexCoords;
	GLint aNormal;
	
	//general uniforms
	GLint uMVP;
	GLint uModelMatrix;
	GLint uNormalMatrix;
	GLint uCameraPosition;
	GLint uAmbientLight;
	//custom properties
	CRDictionary<string, Property*> properices;

	unsigned int active_texture_slot;
	//GLint uColor;
	//GLint uSpecularColor;
	//GLint uShininess;
	//GLint uDiffuseTexture;
	//GLint uSpecularTexture;
	//GLint uShininessTexture;

	Shader(const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

	virtual bool UseLights();
	virtual void TransferLightData(const CRArray<Light*>& lights);

	void AddProperty(Property* prop);
	Property* GetProperty(const string& name);

	GLuint GetProgram();

protected:
	GLuint program;
private:
	GLuint vertex_shader;
	GLuint fragment_shader;

	GLuint Compile(const string& filename);
	void CompileProgram();
};

}