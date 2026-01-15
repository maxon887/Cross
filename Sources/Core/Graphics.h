#pragma once
#include "Cross.h"
#include "Base/Array.h"

namespace cross{

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
