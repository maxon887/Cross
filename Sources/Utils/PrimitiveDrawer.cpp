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
#include "PrimitiveDrawer.h"
#include "GraphicsGL.h"
#include "Game.h"
#include "Screen.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Shaders/Shader.h"
#include "Sprite.h"
#include "Texture.h"
#include "Scene.h"

using namespace cross;

void PrimitiveDrawer::DrawPoint(const Vector2D& pos,const Color& color) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Simple.sha");
	gfxGL->UseShader(shader);
	Camera* cam = game->GetCurrentScreen()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, pos.GetData()));
	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glDrawArrays(GL_POINTS, 0, 1));
}

void PrimitiveDrawer::DrawLine(const Vector2D& p1, const Vector2D& p2, const Color& color) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Simple.sha");
	gfxGL->UseShader(shader);
	float vertices[4] = { p1.x, p1.y, p2.x, p2.y };
	Camera* cam = game->GetCurrentScreen()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glDrawArrays(GL_LINES, 0, 2));
}

void PrimitiveDrawer::DrawRect(const Rect& rect, const Color& color) {
	DrawRect(rect, color, false);
}

void PrimitiveDrawer::DrawRect(const Rect& rect, const Color& color, bool filled) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Simple.sha");
	gfxGL->UseShader(shader);
	float vertices[4 * 2] = { rect.x, rect.y,
		rect.x + rect.width, rect.y,
		rect.x, rect.y + rect.height,
		rect.x + rect.width, rect.y + rect.height };

	Camera* cam = game->GetCurrentScreen()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glEnable(GL_BLEND));
	if(filled) {
		static GLushort indices[] = { 0, 1, 2, 3 };
		SAFE(glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, indices));
	} else {
		static GLushort indices[] = { 0, 1, 3, 2, 0 };
		SAFE(glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_SHORT, indices));
	}
}

void PrimitiveDrawer::DrawCircle(const Vector2D& center, float radius, const Color& color) {
	DrawCircle(center, radius, color, false);
}

void PrimitiveDrawer::DrawCircle(const Vector2D& center, float radius, const Color& color, bool filled) {
	DrawCircle(center, radius, color, filled, 30);
}

void PrimitiveDrawer::DrawCircle(const Vector2D& center, float radius, const Color& color, bool filled, U32 accuracy) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Simple.sha");
	gfxGL->UseShader(shader);
	U32 vertexCount = accuracy;

	// Create a buffer for vertex data
	float* buffer = new float[vertexCount * 2];
	U32 idx = 0;

	// Center vertex for triangle fan
	buffer[idx++] = center.x;
	buffer[idx++] = center.y;

	// Outer vertices of the circle
	U32 outerVertexCount = vertexCount - 1;

	for(U32 i = 0; i < outerVertexCount; ++i) {
		float percent = (i / (float)(outerVertexCount - 1));
		float rad = percent * 2 * PI;

		//Vertex position
		float outer_x = center.x + radius * cos(rad);
		float outer_y = center.y + radius * sin(rad);

		buffer[idx++] = outer_x;
		buffer[idx++] = outer_y;
	}
	Camera* cam = game->GetCurrentScreen()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, buffer));
	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	if(filled) {
		SAFE(glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount));
	} else {
		SAFE(glDrawArrays(GL_LINE_LOOP, 2, vertexCount - 2));
	}
	delete buffer;
}

void PrimitiveDrawer::DrawSprite(Sprite* sprite) {
	DrawSprite(sprite, sprite->GetColor(), false);
}

void PrimitiveDrawer::DrawSprite(Vector2D pos, Sprite* sprite) {
	sprite->SetPosition(pos);
	DrawSprite(sprite, sprite->GetColor(), false);
}

void PrimitiveDrawer::DrawSprite(Sprite* sprite, Color color, bool monochrome) {
	DrawSprite(sprite, color, game->GetCurrentScreen()->GetCamera(), monochrome);
}

void PrimitiveDrawer::DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Texture.sha");
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO));
	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->GetID()));

	gfxGL->UseShader(shader);

	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));

	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * sprite->GetModelMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
}

void PrimitiveDrawer::DrawLine(const Vector3D& p1, const Vector3D& p2, const Color& c) {
	Shader* shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Simple.sha");
	gfxGL->UseShader(shader);
	static const float vertices[6] = { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z };
	Camera* cam = game->GetCurrentScene()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform4fv(shader->uColor, 1, c.GetData()));
	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glDrawArrays(GL_LINES, 0, 2));
}