#pragma once
#include "Shaders/Shader.h"
#include "Base/List.h"

namespace cross{

class MultiLightShader : public Shader {
public:
	MultiLightShader();
	MultiLightShader(const String& vert, const String& frag);

	void Compile() override;
	void Compile(const List<Light*>& lights);
	void Compile(S32 pointCount, S32 spotCount, S32 directionalCount);

	void OnDraw() override;

private:
	Array<LightUniforms> uPointLights		= Array<LightUniforms>();
	Array<LightUniforms> uDirectionalLights = Array<LightUniforms>();
	Array<LightUniforms> uSpotLights		= Array<LightUniforms>();
};

}