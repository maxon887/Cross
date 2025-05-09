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

namespace cross{

template<class BaseClass>
class Factory;

/*	Class containing specific OpenGL features */
class Graphics {
public:
	Factory<Shader>* shader_factory = nullptr;
	
	Graphics();
	~Graphics();

	void Start();
	void Stop();
	void Update();

	Shader* LoadShader(const String& shaderfile);
	
	void RegisterMeshForDrawing(Mesh* mesh);
	void UnregisterMeshForDrawing(Mesh* mesh);

private:
	Array<Mesh*> opaque_meshes;
	Array<Mesh*> transparent_meshes;

	void OnScreenChanged(Screen* newScreen);
};

}
