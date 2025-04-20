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
#include "Internals/GraphicsGL.h"

#include <cstring>

namespace cross{

/*	This class needed to link gpu shader input data to engine cpu core.
	Needed to create any kind of material */
class Shader {
public:
	class Property {
	public:

		enum Type {
			INT,
			FLOAT,
			COLOR,
			VEC3,
			MAT4,
			TEXTURE,
			CUBEMAP,
			UNKNOWN,
		};

		union Value {
			S32 s32;
			float f;
			Color color;
			Vector3D vec3;
			Matrix mat;
			Texture* texture;
			Cubemap* cubemap;

			Value() { memset(this, 0, sizeof(Value)); }
			Value(S32 v) : s32(v) { }
			Value(float v) : f(v) { }
			Value(const Color& v) : color(v) { }
			Value(const Vector3D& v) : vec3(v) { }
			Value(const Matrix& v) : mat(v) { }
			Value(Texture* v) : texture(v) { }
			Value(Cubemap* v) : cubemap(v) { }
		};

		static String TypeToString(Type t);
		static Type StringToType(const String& type);

		String name		= String();
		String glName	= String();
		Type type 		= UNKNOWN;

		Property(String name, String glName);
		Property(String name, String glName, Type type);
		Property(String name, String glName, S32 value);
		Property(String name, String glName, float value);
		Property(String name, String glName, const Color& value);
		Property(String name, String glName, const Vector3D& value);
		Property(String name, String glName, const Matrix& value);
		Property(String name, String glName, Texture* value);
		Property(String name, String glName, Cubemap* value);
		Property(const Property& obj);

		void SetValue(S32 v);
		void SetValue(float v);
		void SetValue(const Color& v);
		void SetValue(const Vector3D& v);
		void SetValue(const Matrix& v);
		void SetValue(Texture* texture);
		void SetValue(Cubemap* cubemap);
		Value& GetValue();

		Property* Clone() const;

		GLint GetID() const;

	engineonly:
		GLint glId = -1;

		Value value;
	};

	//general attributes
	GLint aPosition = -1;
	GLint aTexCoords = -1;
	GLint aColor = -1;
	GLint aNormal = -1;
	GLint aTangent = -1;
	GLint aBitangent = -1;
	//general uniforms
	GLint uMVP = -1;
	GLint uModelMatrix = -1;
	GLint uNormalMatrix = -1;
	GLint uCameraPosition = -1;
	GLint uAmbientLight = -1;

	Shader() = default;
	Shader(const String& vertexFile, const String& fragmentFile);
	virtual ~Shader();

	virtual void Use();
	virtual void Save(const String& file);
	virtual void Compile();
	virtual void OnDraw() { }

	bool IsCompiled() const;
	String& GetFilename();
	void SetFilename(const String& filename);
	String& GetVertexFilename();
	void SetVertexFilename(const String& filename);
	String& GetFragmentFilename();
	void SetFragmentFilename(const String& filename);

	void AddMacro(const String& macro);
	void AddMacro(const String& macro, int value);
	Array<String>& GetMacrosies();

	void AddProperty(String name, String glName);
	void AddProperty(String name, String glName, Property::Type type);
	void AddProperty(String name, String glName, float defValue);
	void AddProperty(String name, String glName, const Color& color);
	void AddProperty(String name, String glName, const Vector3D& vec);
	void AddProperty(String name, String glName, Cubemap* cubemap);
	void AddProperty(const Property& prop);
	Property* GetProperty(const String& name);
	Array<Property>& GetProperties();

protected:
	class LightUniforms {
	public:
		GLint position			= -1;
		GLint direction			= -1;
		GLint color				= -1;

		GLint intensity			= -1;

		GLint cut_off			= -1;
		GLint outer_cut_off		= -1;
	};

	GLuint program				= 0;
	Array<String> macrosies		= Array<String>();
	bool compiled				= false;
	//custom uniforms
	Array<Property> properties	= Array<Property>();

	GLuint GetProgram() const;

private:
	GLuint vertex_shader		= 0;
	GLuint fragment_shader		= 0;
	String vertex_filename		= "";
	String fragment_filename	= "";
	File* vertex_file			= nullptr;
	File* fragment_file			= nullptr;
	String filename				= "noname";

	GLuint CompileShader(GLuint type, File* file);
	void CompileProgram();
	void FreeResources();
};

}
