#include "MultiLightScene.h"
#include "Light.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include "Shaders/MultiLightShader.h"
#include "Mesh.h"
#include "Transform.h"

void MultiLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -30.f));

	for(U32 i = 0; i < 8; ++i){
		Entity* light = CREATE Entity("PointLight_" + String(i));
		light->AddComponent(CREATE Transform());
		Light* lightComponent = CREATE Light(Light::Type::POINT);
		light->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		lightComponent->SetColor(Color::Red);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	for(U32 i = 0; i < 1; ++i){
		Entity* light = CREATE Entity("DirectionalLight_" + String(i));
		light->AddComponent(CREATE Transform());
		Light* lightComponent = CREATE Light(Light::Type::DIRECTIONAL);
		light->GetTransform()->SetDirection(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)));
		lightComponent->SetColor(Color::Green);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	for(U32 i = 0; i < 4; ++i){
		Entity* light = CREATE Entity("SpotLight_" + String(i));
		light->AddComponent(CREATE Transform());
		Light* lightComponent = CREATE Light(Light::Type::SPOT);
		light->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		light->GetTransform()->LookAt(Vector3D(0.f));
		lightComponent->SetColor(Color::Blue);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	shader = CREATE MultiLightShader();
	shader->AddUniform("Transparency", "uTransparency", 1.f);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
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
		clone->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
	AddEntity(cubes);
}

void MultiLightScene::Stop(){
	delete material;
	delete shader;
	delete cube;
	DemoScene::Stop();
}