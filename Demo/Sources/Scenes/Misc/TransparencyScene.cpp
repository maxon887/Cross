#include "TransparencyScene.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

#include <cmath>

void TransparencyScene::Start(){
	DemoScene::Start();

	SetAmbientColor(Color(0.15f));

	//lights
	Light* lightComponent = CREATE Light(Light::Type::POINT);
	lightComponent->SetIntensity(0.01f);
	light = CREATE Entity("PointLight");
	light->AddComponent(CREATE Transform());
	light->AddComponent(lightComponent);
	AddEntity(light);

	road_shader = CREATE MultiLightShader();
	road_shader->AddUniform("Transparency", "uTransparency", 1.f);
	road_shader->AddMacro("USE_DIFFUSE_MAP");
	road_shader->AddMacro("USE_TILLING_FACTOR");
	road_shader->AddUniform("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddUniform("Tilling Factor", "uTillingFactor", 1.f);
	road_shader->AddUniform("Specular", "uSpecular", 0.5f);
	road_shader->AddUniform("Shininess", "uShininess", 0.5f * 128.f);
	road_shader->Compile();
	Texture* roadDiffuse = GetTexture("Textures/RoadDiffuse.png");
	roadDiffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = CREATE Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", roadDiffuse);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	Entity* road = LoadPrimitive(Model::Primitive::PLANE);
	road->GetComponent<Mesh>()->SetMaterial(road_mat);
	road->GetTransform()->SetScale(15.f);
	AddEntity(road);

	grass_shader = CREATE MultiLightShader();
	grass_shader->AddUniform("Transparency", "uTransparency", 1.f);
	grass_shader->AddMacro("USE_DIFFUSE_MAP");
	grass_shader->AddMacro("USE_CUTOUT");
	grass_shader->AddUniform("Diffuse Texture", "uDiffuseTexture");
	grass_shader->AddUniform("Specular", "uSpecular", 0.5f);
	grass_shader->AddUniform("Shininess", "uShininess", 0.5f * 128.f);
	grass_shader->Compile();
	Entity* grass = LoadPrimitive(Model::Primitive::PLANE);
	grass_mat = CREATE Material(grass_shader);
	grass_mat->SetPropertyValue("Diffuse Texture", GetTexture("Textures/GrassDiffuse.png"));
	grass_mat->EnableTransparency(true);
	grass->GetComponent<Mesh>()->SetMaterial(grass_mat);
	grass->GetTransform()->SetRotate(Quaternion(Vector3D::Up, 0.f));
	AddEntity(grass);

	for(U32 i = 0; i < 10; ++i){
		Entity* clone = grass->Clone();
		clone->GetTransform()->SetRotate(Quaternion(Vector3D::Up, 0.f));
		clone->GetTransform()->SetPosition(Vector3D(Random(-5.f, 5.f), .5f, Random(-5.f, 5.f)));
		Quaternion quat(Vector3D::Up, Random(360.f));
		clone->GetTransform()->SetRotate(quat);
		AddEntity(clone);
	}

	sphere_shader = CREATE MultiLightShader();
	sphere_shader->AddUniform("Transparency", "uTransparency", 1.f);
	sphere_shader->AddUniform("Color", "uDiffuseColor", Color::Blue);
	sphere_shader->AddUniform("Specular", "uSpecular", 0.5f);
	sphere_shader->AddUniform("Shininess", "uShininess", 0.5f * 128.f);
	sphere_shader->Compile();

	sphere_mat = CREATE Material(sphere_shader);
	sphere_mat->SetPropertyValue("Transparency", 0.5f);
	sphere_mat->EnableTransparency(true);

	Entity* sphere = LoadPrimitive(Model::Primitive::SPHERE);
	sphere->GetComponent<Mesh>()->SetMaterial(sphere_mat);
	AddEntity(sphere);
	sphere->GetTransform()->SetPosition(Vector3D(0.f, 1.f, 0.f));
}

void TransparencyScene::Stop(){
	delete sphere_mat;
	delete sphere_shader;
	delete grass_mat;
	delete grass_shader;
	delete road_mat;
	delete road_shader;
	DemoScene::Stop();
}

void TransparencyScene::Update(float sec){
	DemoScene::Update(sec);
	light->GetTransform()->SetPosition(Vector3D(cosf(game->GetRunTime() / 2.f)*3.f, 2.f, sinf(game->GetRunTime() / 2.f)*3.f));
}