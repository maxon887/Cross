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
#include "TriangleAdvanced.h"
#include "Launcher.h"
#include "SOIL\SOIL.h"
#include "Matrix.h"

GLfloat vertices[] = {
	// Positions				// Colors           // Texture Coords
	 0.5f,  0.5f, 0.0f, 1.0f,	1.0f, 0.0f,	0.0f,	1.0f, 0.0f,   // Top Right
	 0.5f, -0.5f, 0.0f,	1.0f,	0.0f, 1.0f,	0.0f,	1.0f, 1.0f,   // Bottom Right
	-0.5f, -0.5f, 0.0f,	1.0f,	0.0f, 0.0f,	1.0f,	0.0f, 1.0f,   // Bottom Left
	-0.5f,  0.5f, 0.0f,	1.0f,	1.0f, 1.0f,	0.0f,	0.0f, 0.0f    // Top Left 
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};
/*
GLfloat transform[] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};*/

static Matrix* transform;

static GLuint program;
static GLuint VBO;
static GLuint texture1;
static GLuint texture2;

void TriangleAdvanced::Start(){
	Shader* vertexShader = new Shader("advanced.vert");
	Shader* fragmentShader = new Shader("advanced.frag");
	gfx3D->AttachShader(vertexShader);
	gfx3D->AttachShader(fragmentShader);
	program = gfx3D->CompileProgram();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLint posLoc = glGetAttribLocation(program, "aPosition");
	GLint colLoc = glGetAttribLocation(program, "aColor");
	GLint texLoc = glGetAttribLocation(program, "aTexCoord");
	glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
	glVertexAttribPointer(colLoc, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	glVertexAttribPointer(texLoc, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(colLoc);
	glEnableVertexAttribArray(texLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//texturing
	File* texFile1 = launcher->LoadFile("container.jpg");
	int width1, height1;
	byte* image1 = SOIL_load_image_from_memory(texFile1->data, texFile1->size, &width1, &height1, 0, SOIL_LOAD_RGBA);
	delete texFile1;
	File* texFile2 = launcher->LoadFile("awesomeface.jpg");
	int width2, height2;
	byte* image2 = SOIL_load_image_from_memory(texFile2->data, texFile2->size, &width2, &height2, 0, SOIL_LOAD_RGBA);
	delete texFile2;
	
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);


	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	transform = new Matrix();
	transform->Translate(Vector2D(0.3f, 0.3f));
	//transform->RotateZ(30);
}

void TriangleAdvanced::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	static float time = 0;
	time += sec;
	transform->RotateZ(time * 8);
	GLuint transformLoc = glGetUniformLocation(program, "uTransform");
	glUniformMatrix4fv(transformLoc, 1, GL_FLOAT, transform->GetData());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	GLuint texPos1 = glGetUniformLocation(program, "uTexture1");
	glUniform1i(texPos1, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	GLuint texPos2 = glGetUniformLocation(program, "uTexture2");
	glUniform1i(texPos2, 1);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}