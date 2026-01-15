#include "PointLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/SingleLightShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"
	
void PointLightScene::Start(){
	DemoScene::Start();

	GetCamera()->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -30.0f));

	Entity* light = CREATE Entity("PointLight");
	light->AddComponent(CREATE Transform());
	light->AddComponent(CREATE Light(Light::Type::POINT));
	AddEntity(light);

	shader = CREATE SingleLightShader("Engine/Shaders/Sources/SingleLight.vert", "Engine/Shaders/Sources/SingleLight.frag");
	shader->AddMacro("POINT_LIGHT");
	shader->AddMacro("POINT_OR_SPOT_LIGHT");
	shader->AddUniform("Diffuse Texture", "uDiffuseTexture");
	shader->AddUniform("Specular Map", "uSpecularMap");
	shader->AddUniform("Shininess", "uShininess");
	shader->Compile();
	material = CREATE Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("Textures/ContainerDiffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("Textures/ContainerSpecular.png"));
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	Entity* cubes = CREATE Entity("Cubes");
	cubes->AddComponent(CREATE Transform());
	for(U32 i = 0; i < 50; ++i){
		Entity* clone = cube->Clone();
		clone->SetName(clone->GetName() + String(i));
		clone->GetTransform()->SetPosition(Vector3D(Random(-15.f, 15.f), Random(-15.f, 15.f), Random(-15.f, 15.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
	AddEntity(cubes);
}

void PointLightScene::Stop(){
	delete material;
	delete shader;
	delete cube;
	DemoScene::Stop();
}