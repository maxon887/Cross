#include "DirectionalLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/SingleLightShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"

void DirectionalLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -8.f));

	Entity* light = CREATE Entity("DirectionalLight");
	light->AddComponent(CREATE Transform(Vector3D(0.f, 0.f, 1.f)));
	light->AddComponent(CREATE Light(Light::Type::POINT));
	AddEntity(light);

	shader = CREATE SingleLightShader("Engine/Shaders/Sources/SingleLight.vert", "Engine/Shaders/Sources/SingleLight.frag");
	shader->AddMacro("DIRECT_LIGHT");
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
	for(U32 i = 0; i < 10; ++i){
		Entity* clone = cube->Clone();
		clone->SetName(clone->GetName() + String(i));
		clone->GetTransform()->SetPosition(Vector3D(Random(-3.f, 3.f), Random(-3.f, 3.f), Random(-3.f, 3.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
	AddEntity(cubes);
}

void DirectionalLightScene::Stop(){
	delete cube;
	delete material;
	delete shader;
	DemoScene::Stop();
}