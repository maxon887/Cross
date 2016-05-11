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
#include "Transformable.h"
#include "Mesh.h"

namespace cross{

class Model : public Transformable{
public:
	enum Type{
		SOLID,
		MATERIAL,
		TEXTURED
	};
/*
	Model(Mesh* mesh, const Color& color);
	Model(Mesh* mesh, const Material& material);
	Model(Mesh* mesh, Texture* diffuse);
	Model(Mesh* mesh, Texture* diffuse, Texture* specular);
	Model(CRArray<Mesh*>& meshes, const Color& color);
	Model(CRArray<Mesh*>& meshes, const Material& material);
	Model(CRArray<Mesh*>& meshes, Texture* diffuse);
	Model(CRArray<Mesh*>& meshes, Texture* diffuse, Texture* specular);*/

	//Solid constructor
	Model(CRArray<Mesh*>& meshes, const Color& color);
	//Material constructor
	Model(CRArray<Mesh*>& meshes, const Material& material);
	//Textured constructor
	Model(CRArray<Mesh*>& meshes, CRArray<Texture*>& diffuseMaps,	CRArray<Texture*>& specularMaps);
	
	~Model();

	Type GetType();
	Color GetColor();
	Texture* GetDiffuseTexture();
	Material* GetMaterial();
	bool HasSpecularMap();
	Texture* GetSpecularTexture();
	void AddDiffuseTexture(Texture* texture);
	void AddSpecularTexture(Texture* texture);

	int GetPolyCount();
	Model* Clone();

protected:
	friend Graphics3D;

	Type type;
	CRArray<Mesh*> meshes;
	CRArray<Texture*> diffuse_maps;
	CRArray<Texture*> specular_maps;
	Material* material;
	Color* color;
	bool original;

	//Copy constructor
	Model(Model& obj);
	Model(Type type);
};

}