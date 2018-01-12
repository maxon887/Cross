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

#if defined(_DEBUG) || ! defined(NDEBUG)
#   define CROSS_DEBUG
#endif

/*	Access modifier that allow usage only inside engine classes */
#define engineonly protected: CROSS_FRIENDLY protected

#define CROSS_ASSERT(condition, message, ...)						\
if(!(condition)) {													\
	EvokeAlert(__FILE__, __LINE__, message, ##__VA_ARGS__);			\
}

#define CROSS_FAIL(condition, message, ...)							\
if(!(condition)) {													\
	EvokeAlert(__FILE__, __LINE__, message, ##__VA_ARGS__);			\
	return;															\
}

#define CROSS_RETURN(condition, value, message, ...)				\
if(!(condition)) {													\
	EvokeAlert(__FILE__, __LINE__, message, ##__VA_ARGS__);			\
	return value;													\
}

#if defined(__APPLE__) || defined(__APPLE_CC__)
#	define IOS 1
#endif

#include <cstdint>

#undef _CRT_USE_WINAPI_FAMILY_DESKTOP_APP

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
}

void EvokeAlert(const char* filename, unsigned int line, const char* msg, ...);

#include "Internals/MemoryManager.h"
#include "Math/All.h"
#include "Color.h"

#include <string>
#include <list>
#include <map>
#include <set>

#include "Experimental/ArraySTD.h"

namespace cross{

typedef int8_t		S8;
typedef uint8_t		U8;
typedef int16_t		S16;
typedef uint16_t	U16;
typedef int32_t		S32;
typedef uint32_t	U32;
typedef int64_t		S64;
typedef uint64_t	U64;
typedef uint8_t		Byte;

template<typename Type>
using Array = ArraySTD<Type>;
template<typename Type>
using List = std::list<Type>;
template<typename Key, typename Value>
using Dictionary = std::map<Key, Value>;
template<typename Type>
using Set = std::set<Type>;

class Game;
class System;
class GraphicsGL;
class Graphics3D;
class Input;
class Config;
class Audio;
class Sound;
class Screen;
class Scene;
class File;
class Camera;
class Camera2D;
class Shader;
class Entity;
class Material;
class Mesh;
class Model;
class Texture;
class Light;
class Cubemap;
class Component;
class Collider;
class Transform;

extern Game*		game;
extern System*		system;
extern GraphicsGL*	gfxGL;
extern Audio*		audio;
extern Input*		input;
extern Config*		config;

static const char*	version = "4.0.1";

}

/*	This function must implement every game */
cross::Game* CrossMain();

using namespace std;

#define CROSS_FRIENDLY	\
friend Shader;			\
friend Material;		\
friend Mesh;			\
friend GraphicsGL;		\
friend Game;			\
friend System;			\
friend Audio;			\
friend Sound;			\
friend Input;			\
friend Config;			\
friend Scene;			\
friend Screen;			
