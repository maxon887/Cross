#pragma once
#include "Cross.h"

namespace cross{

/*	Special type of texture which represents 360 observable texture.
	Created by 6 faces(top, right, back etc.) */
class Cubemap {
public:
	Cubemap(const String& right,
			const String& left,
			const String& top,
			const String& bottom,
			const String& back,
			const String& front);
	~Cubemap();

	U64 GetTextureID() const;

private:
	U64 textureID = 0;
};

}