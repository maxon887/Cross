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
#include "GraphicsGL.h"
#include "Launcher.h"
#include "Utils/Debugger.h"
#include "Sprite.h"
#include "Font.h"
#include "Camera2D.h"
#include "File.h"
#include "Texture.h"
#include "Shaders/Shader.h"

#include "Libs/SOIL/SOIL.h"
#include "Libs/FreeType/ft2build.h"
#include "Libs/TinyXML/tinyxml.h"

#include <math.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

using namespace cross;

Graphics2D::Graphics2D() :
	camera(nullptr)
{
	launcher->LogIt("Graphics2D::Graphics2D()");
	this->default_font = new Font("Engine/Fonts/VeraMono.ttf", 50, Color::White);
	default_camera = new Camera2D();
}

Graphics2D::~Graphics2D(){
	launcher->LogIt("Graphics2D::~Graphics2D");
	delete default_font;
	delete default_camera;
}

void Graphics2D::SetCamera(Camera2D* camera){
	this->camera = camera;
}

Camera2D* Graphics2D::GetCamera() {
	if(camera) {
		return camera;
	} else {
		return default_camera;
	}
}

Camera2D* Graphics2D::GetDefaultCamera(){
	return default_camera;
}

void Graphics2D::DrawPoint(Vector2D pos, Color color){
	Shader* shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	gfxGL->UseShader(shader);
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, pos.GetData()));
	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glDrawArrays(GL_POINTS, 0, 1));
}

void Graphics2D::DrawLine(Vector2D p1, Vector2D p2, Color color){
	Shader* shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	gfxGL->UseShader(shader);
	float vertices[4] = { p1.x, p1.y, p2.x, p2.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glDrawArrays(GL_LINES, 0, 2));
}

void Graphics2D::DrawRect(Rect rect, Color color){
	DrawRect(rect, color, false);
}

void Graphics2D::DrawRect(Rect rect, Color color, bool filled){
	Shader* shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	gfxGL->UseShader(shader);
	float vertices[5 * 2] = {	rect.x, rect.y,
								rect.x + rect.width, rect.y,
								rect.x + rect.width, rect.y + rect.height,
								rect.x, rect.y + rect.height,
								rect.x, rect.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	if(filled){
		static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices));
	}else{
		SAFE(glDrawArrays(GL_LINE_STRIP, 0, 5));
	}
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color){
	DrawCircle(center, radius, color, false);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled){
	DrawCircle(center, radius, color, filled, 30);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled, int accuracy){
	Shader* shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	gfxGL->UseShader(shader);
	int vertexCount = accuracy;

	// Create a buffer for vertex data
	float* buffer = new float[vertexCount * 2]; 
	int idx = 0;

	// Center vertex for triangle fan
	buffer[idx++] = center.x;
	buffer[idx++] = center.y;

	// Outer vertices of the circle
	int outerVertexCount = vertexCount - 1;

	for(int i = 0; i < outerVertexCount; ++i){
		float percent = (i / (float)(outerVertexCount - 1));
		float rad = percent * 2 * PI;

		//Vertex position
		float outer_x = center.x + radius * cos(rad);
		float outer_y = center.y + radius * sin(rad);

		buffer[idx++] = outer_x;
		buffer[idx++] = outer_y;
	}
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, buffer));
	//delete buffer;
	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	if(filled){
		SAFE(glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount));
	}else{
		SAFE(glDrawArrays(GL_LINE_LOOP, 2, vertexCount - 2));
	}
	delete buffer;
}

void Graphics2D::DrawText(Vector2D pos, string textStr){
	DrawText(pos, textStr, this->default_font);
}

void Graphics2D::DrawText(Vector2D pos, const string &textStr, Font* font){
	const char* text = textStr.c_str();

	while(*text){
		Sprite* ch = font->GetChar(*text);
		float advance = font->GetCharAdvance(*text);
		ch->SetPosition(pos);
		DrawSprite(ch, font->GetColor(), true);
		pos.x += advance;
		text++;
	}
}

void Graphics2D::DrawSprite(Sprite* sprite){
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Vector2D pos, Sprite* sprite){
	sprite->SetPosition(pos);
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, bool monochrome){
	DrawSprite(sprite, color, GetCamera(), monochrome);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome){
	Shader* shader = nullptr;
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO));
	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->GetID()));
	if(monochrome){
		shader = gfxGL->GetShader(DefaultShader::MONOCHROME);
	}else{
		shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	}
	gfxGL->UseShader(shader);

	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));

	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * sprite->GetModelMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
	
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
}

Texture* Graphics2D::LoadTexture(const string& filename){
	auto it = loaded_textures.begin();
	for( ; it != loaded_textures.end(); it++){
		if( (*it).first->GetName() == filename ){
			break;
		}
	}

	if(it != loaded_textures.end()){
		(*it).second++;
		return (*it).first->Clone();
	}else{
		Texture* newTexture = LoadTexture(filename, Texture::Filter::LINEAR);
		pair<Texture*, int> pair;
		pair.first = newTexture;
		pair.second = 1;
		loaded_textures.push_back(pair);
		return newTexture->Clone();
	}
}

void Graphics2D::ReleaseTexture(const string& filename, GLuint* id){
	auto it = loaded_textures.begin();
	for( ; it != loaded_textures.end(); it++){
		if( (*it).first->GetName() == filename ){
			break;
		}
	}

	if(it != loaded_textures.end()){
		(*it).second--;
		if((*it).second <= 0){
			Texture* texture = (*it).first;
			loaded_textures.erase(it);
			delete texture;
		}
	}else{
		SAFE(glDeleteTextures(1, id));
	}
}

Texture* Graphics2D::LoadTexture(const string& filename, Texture::Filter filter){
	Debugger::Instance()->StartCheckTime();
	int width, height, channels;
	File* textureFile = launcher->LoadFile(filename);
	CRByte* image = SOIL_load_image_from_memory(textureFile->data, textureFile->size, &width, &height, &channels, SOIL_LOAD_AUTO);
	delete textureFile;
	if(image == NULL){
		throw CrossException("SOL can't convert file:\n Pay attention on image color channels");
	}
	int newWidth = 1;
	int newHeight = 1;
	while(newWidth < width) {
		newWidth *= 2;
	}
	while(newHeight < height) {
		newHeight *= 2;
	}
	if(newWidth != width || newHeight != height){
		launcher->LogIt("Not power of 2 texture. Performance issue!");
		CRByte* newImage = (CRByte*)malloc(channels * newWidth * newHeight);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * newWidth * channels, image + i * width * channels, width * channels);
			//Clamp to edge effect
			if(newWidth > width){
				memcpy(newImage + i * newWidth * channels + width * channels, image + i * width * channels, channels);
			}
		}
		//Clamp to edge effect
		if(newHeight > height){
			memcpy(newImage + height * channels * newWidth, image + (height - 1) * width * channels, width * channels);
		}
		width = newWidth;
		height = newHeight;
		image = newImage;
	}
	Texture* texture = CreateTexture(image, channels, width, height, filter);
	texture->SetName(filename);
	string debugMsg = "Texure(" + filename + ") loaded in ";
	Debugger::Instance()->StopCheckTime(debugMsg);
	return texture;
}

Texture* Graphics2D::CreateTexture(CRByte* data, int channels, int width, int height){
	return CreateTexture(data, channels, width, height, Texture::Filter::LINEAR);
}

Texture* Graphics2D::CreateTexture(int channels, int width, int height, Texture::Filter filter){
	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(channels) {
	case 1:
		SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0));
		break;
	case 3:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));
		break;
	case 4:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
		break;
	default:
		throw CrossException("Wrong texture channel count");
	}
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	Texture* texture = new Texture(id, width, height, filter);
	return texture;
}

Texture* Graphics2D::CreateTexture(CRByte* data, int channels, int width, int height, Texture::Filter filter){
	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(channels) {
	case 1:
		SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data));
		break;
	case 3:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		break;
	case 4:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		break;
	default:
		throw CrossException("Wrong texture channel count");
	}
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	Texture* texture = new Texture(id, width, height, filter);
	return texture;
}

void Graphics2D::LoadSprites(CRDictionary<string, Sprite*>& output, Texture* texture, string xmlFilename){
	File* xmlFile = launcher->LoadFile(xmlFilename);
	TiXmlDocument xml;
	CRByte* source = new CRByte[xmlFile->size + 1]; // +1 for null terminated string
	memcpy(source, xmlFile->data, xmlFile->size);
	source[xmlFile->size] = 0;
	delete xmlFile;
	xml.Parse((const char*)source, 0, TIXML_ENCODING_UTF8);
	delete source;

	TiXmlHandle xmlDoc(&xml);
	TiXmlElement* root;
	TiXmlElement* element;

	root = xmlDoc.FirstChildElement("TextureAtlas").Element();
	if(root){
		element = root->FirstChildElement("sprite");
		while(element){
			string name = element->Attribute("n");
			float xPos = stof(element->Attribute("x"));
			float yPos = stof(element->Attribute("y"));
			float width = stof(element->Attribute("w"));
			float height = stof(element->Attribute("h"));
			Rect rect(xPos, yPos, width, height);
			Sprite* sprite = new Sprite(texture, rect);
			output[name] = sprite;
			element = element->NextSiblingElement("sprite");
		}
	}else{
		throw CrossException("XML empty root element");
	}
}

void Graphics2D::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GetCamera()->Update(sec);
}

Font* Graphics2D::GetDefaultFont(){
	return default_font;
}