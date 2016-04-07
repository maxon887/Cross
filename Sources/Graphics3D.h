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
#include "Graphics3D/TriangleShaders.h"

namespace cross{

class Graphics3D : public GraphicsGL{
public:
	Graphics3D();

	Camera* GetCamera();
	Mesh* LoadMesh(string filename);
	void DrawMesh(Mesh* mesh, const Matrix& model);
private:
	Camera* camera;
	TriangleShaders* triangle_shader;
};

}
