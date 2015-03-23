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
	
#include "Graphics.h"
#include "Image.h"

Graphics::Graphics(Game* game){
	this->game = game;
	Launcher* launcher = game->launcher;
	glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, launcher->GetTargetWidth(), launcher->GetTargetHeight(), 0, 1, -1);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics::Clear(float r, float g, float b){
	glClearColor(r, g, b, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

Image* Graphics::LoadImage(const char* filename){
	//if(strcmp(prev_tex_filename, filename)){
		//create new Image
	//}
    
    Launcher* launcher = game->launcher;
    const char* platformPath = launcher->DataPath();
    char* absPath = (char*)malloc(strlen(platformPath) + strlen(filename) + 1);
    strcpy(absPath, platformPath);
    strcat(absPath, "/");
    strcat(absPath, filename);
    
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//prev_tex_id = textureID;
	//strcpy(prev_tex_filename, filename);
	int width;
	int height;

	unsigned char* image = SOIL_load_image(absPath, &width, &height, 0, SOIL_LOAD_RGBA);
	textureID = SOIL_create_OGL_texture(image, width, height, 4, 0, SOIL_FLAG_POWER_OF_TWO);
	SOIL_free_image_data(image);

	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	Rect region(0, 0, width, height);
	return new Image(textureID, width, height, region);
}

void Graphics::DrawTargetImage(PointX p, Image* img){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->GetTextureID());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(p.x, p.y, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	float* verts = img->GetVertices(); 
	glVertexPointer(2, GL_FLOAT, 16, verts);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	verts += 2;
	glTexCoordPointer(2, GL_FLOAT, 16, verts);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, Image::indices);

}


















void Graphics::Test(){
	GLfloat width = game->launcher->GetTargetWidth();
	GLfloat height = game->launcher->GetTargetHeight();

	//	GLfloat verts[] = {	0.0f, 0.0f,
    //                        width - 1, 0.0f,
    //                        width/2, height - 1 };
    GLfloat verts[] = { width - 1, height - 1,
        0, height - 1,
        width / 2, 0 };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
