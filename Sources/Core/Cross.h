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

/*	Access modifier that allow usage only inside engine classes */
#define engineonly public

#define CROSS_ASSERT(condition, message, ...)									\
if(!(condition)) {																\
	cross::os->Alert(message, __FILE__, __LINE__, ##__VA_ARGS__);				\
}

#define CROSS_FAIL(condition, message, ...)										\
if(!(condition)) {																\
	cross::os->Alert(message, __FILE__, __LINE__, ##__VA_ARGS__);				\
	return;																		\
}

#define CROSS_RETURN(condition, value, message, ...)							\
if(!(condition)) {																\
	cross::os->Alert(message, __FILE__, __LINE__, ##__VA_ARGS__);				\
	return value;																\
}

#include <cstdint>

namespace cross {
	typedef int8_t		S8;
	typedef uint8_t		U8;
	typedef int16_t		S16;
	typedef uint16_t	U16;
	typedef int32_t		S32;
	typedef uint32_t	U32;
	typedef int64_t		S64;
	typedef uint64_t	U64;
	typedef uint8_t		Byte;
	typedef size_t		Size;
}

#include "Internals/MemoryManager.h"

namespace cross{

class Game;
class System;
class Graphics;
class GraphicsGL;
class Input;
class Config;
class Audio;
class Sound;
class Screen;
class Scene;
class File;
class Camera;
class Shader;
class Entity;
class Material;
class Mesh;
class Model;
class Texture;
class Light;
class Component;
class Transform;
class Cubemap;
class Color;

extern Game*		game;
extern System*		os;
extern Graphics*	gfx;
extern GraphicsGL*	gfxGL;
extern Audio*		audio;
extern Input*		input;
extern Config*		config;

static const char*	version = "5.0.0";

}

/* Main function of the Cross Engine. This function must implement every game */
cross::Game* CrossMain();