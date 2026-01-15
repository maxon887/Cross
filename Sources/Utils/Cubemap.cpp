#include "Cubemap.h"
#include "Texture.h"
#include "Internals/GraphicsGL.h"

using namespace cross;

Cubemap::Cubemap(	const String& right,
					const String& left,
					const String& top,
					const String& bottom,
					const String& back,
					const String& front)
{
	SAFE(glGenTextures(1, (GLuint*)&textureID));
	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)textureID));

	int width, height, channels;
	Byte* image;

	image = Texture::LoadRawData(right, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete[] image;
	image = Texture::LoadRawData(left, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = Texture::LoadRawData(top, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = Texture::LoadRawData(bottom, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = Texture::LoadRawData(back, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = Texture::LoadRawData(front, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;

	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

Cubemap::~Cubemap(){
	SAFE(glDeleteTextures(1, (GLuint*)&textureID));
}

U64 Cubemap::GetTextureID() const{
	return textureID;
}