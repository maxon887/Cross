#pragma once
#include "Cross.h"

namespace cross{

class Cubemap;

class Skybox {
public:
	Skybox(Cubemap* cubemap);
	~Skybox();

	void Draw();

private:
	Cubemap* cubemap	= nullptr;
	Shader* shader		= nullptr;
	Material* material	= nullptr;
	Entity* box			= nullptr;
	S32 mvpID			= 0;
};

}