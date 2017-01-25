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
#include "Graphics3D.h"

namespace cross{

/*	This class represent 3D object in scene.
	3D model consists of Meshes, Materials and Shaders.
	Model can be loaded through Graphics3D class. */
class Model : public Transformable{
public:
	enum Format{
		FBX,
		UNKNOW
	};

	Model(const string& name);
	Model(Model& obj);
	~Model();

	virtual void Draw();

	string GetName();
	string GetFilePath();
	Format GetFormat();
	void Initialize();
	void FaceCulling(bool enabled);
	void AlphaBlending(bool enabled);
	void SetStencil(Graphics3D::StencilBehaviour behaviour);

	void AddMesh(Mesh* mesh);
	Mesh* GetMesh(const string& name);
	void SetMaterial(Material* material);
	Material* GetMaterial();

	Model* Clone();

protected:
	string name;
	string filepath;
	Format format;
	Array<Mesh*> meshes;
	Material* material;
	bool original;
	bool face_culling;
	bool alpha_blending;
	Graphics3D::StencilBehaviour stencil;
};

}