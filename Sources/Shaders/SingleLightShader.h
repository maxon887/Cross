#pragma once
#include "Shaders/Shader.h"

namespace cross{

class SingleLightShader : public Shader {
public:
	LightUniforms light_attribs = LightUniforms();

	SingleLightShader() = default;
	SingleLightShader(const String& vertex, const String& fragment);

	void Compile() override;
	void OnDraw() override;
};

}