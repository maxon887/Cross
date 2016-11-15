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

namespace cross{

class Model : public Transformable{
public:
	enum Format{
		FBX,
		UNKNOW
	};

	Model(const string& name);
	~Model();

	virtual void Draw();

	string GetName();
	string GetFilePath();
	Format GetFormat();
	U32 GetPolyCount();
	void FaceCulling(bool enabled);

	void AddMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	Material* GetMaterial();

	Model* Clone();

protected:
	string name;
	string filepath;
	Format format;
	CRArray<Mesh*> meshes;
	Material* material;
	bool original;

	//Copy constructor
	Model(Model& obj);
};

}