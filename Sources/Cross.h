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
#include "MemoryManager.h"
#include "Exception.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include "Color.h"
#include "Rect.h"

#include <string>
#include <vector>
#include <list>

#define CRByte		unsigned char
#define CRArray		std::vector
#define CRList		std::list

#if defined(__APPLE__) || defined(__APPLE_CC__)
#define IOS true
#endif

namespace cross{

class Game;
class Launcher;
class GraphicsGL;
class Graphics2D;
class Graphics3D;
class Input;
class Config;
class Debuger;
class Texter;
class Sprite;
class Audio;
class Screen;
class File;
class Font;

extern Game* game;
extern Launcher* launcher;
extern GraphicsGL* gfxGL;
extern Graphics2D* gfx2D;
extern Graphics3D* gfx3D;
extern Input* input;
extern Config* config;
extern Debuger* debuger;

}

cross::Game* CrossMain(cross::Launcher* launcher);

using namespace std;