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
#include "AdvancedScreen.h"
#include "Launcher.h"
#include "SOIL\SOIL.h"
#include "Utils\Misc.h"
#include "cube_verts.h"
#include "Camera.h"
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

static GLuint program;
static GLuint VBO;
static GLuint texture1;
static GLuint texture2;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

static Camera* camera;

void AdvancedScreen::Start(){
	Shader* vertexShader = new Shader("advanced.vert");
	Shader* fragmentShader = new Shader("advanced.frag");
	gfx3D->AttachShader(vertexShader);
	gfx3D->AttachShader(fragmentShader);
	program = gfx3D->CompileProgram();

	camera = new Camera();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLint posLoc = glGetAttribLocation(program, "aPosition");
	GLint texLoc = glGetAttribLocation(program, "aTexCoord");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(texLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(posLoc);
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
	//glGenerateMipmap(GL_TEXTURE_2D);
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

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);;
}

void AdvancedScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float time = 0;
	time += sec;

	mat4 model;
	model = rotate(model, time * 1.0f, vec3(0.5f, 1.0f, 0.0f));
	//mat4 view;
	//view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	mat4 projection = perspective(45.0f, launcher->DeviceAspect(), 0.1f, 100.0f);

	GLuint modelLoc = glGetUniformLocation(program, "uModel");
	GLuint viewLoc = glGetUniformLocation(program, "uView");
	GLuint projectionLoc = glGetUniformLocation(program, "uProjection");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	GLuint texPos1 = glGetUniformLocation(program, "uTexture1");
	glUniform1i(texPos1, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	GLuint texPos2 = glGetUniformLocation(program, "uTexture2");
	glUniform1i(texPos2, 1);

	//camera 
	/*
	vec3 cameraPosition(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	vec3 cameraDirection = normalize(cameraPosition - cameraTarget);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(glm::cross(cameraUp, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);*/

	GLfloat radius = 10.0f;
	GLfloat camX = sin(time) * radius;
	GLfloat camZ = cos(time) * radius;
	//view = lookAt(vec3(camX, 0.0f, camZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	camera->Update(sec);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera->GetMatrix());



	for(int i = 0; i < 10; i++){
		mat4 model;
		model = translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}