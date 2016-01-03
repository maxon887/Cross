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
#include "Graphics2D.h"
#include "Launcher.h"
#include "Shader.h"
#include "File.h"

#include "SOIL/SOIL.h"

using namespace cross;

Graphics2D::Graphics2D(){
	launcher->LogIt("Graphics2D::Graphics2D()");
	vertex_shader = new Shader("Engine/2d.vert");
	fragment_shader = new Shader("Engine/2d.frag");
	AttachShader(vertex_shader);
	AttachShader(fragment_shader);
	CompileProgram();
}

Graphics2D::~Graphics2D(){

}

byte* Graphics2D::LoadImageInternal(string filename, GLuint* textureID, int* width, int* height){
	File* imageFile = launcher->LoadFile(filename);
	byte* image = SOIL_load_image_from_memory(imageFile->data, imageFile->size, width, height, 0, SOIL_LOAD_RGBA);
	delete imageFile;

	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	if(image == NULL){
		string msg = "Can't load file: " + filename;
		throw msg;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return image;
}