#pragma once

/*	Access modifier that allow usage only inside engine classes */
#define engineonly public

#include <cstdint>
#include <cstddef>

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

class String;
template<typename Type>
class Array;
template<class BaseClass>
class Factory;

class Vector2D;
class Vector3D;
class Vector4D;

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