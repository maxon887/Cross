#include "DepthScene.h"
#include "Entity.h"
#include "Light.h"
#include "Game.h"
#include "Material.h"
#include "Config.h"
#include "Mesh.h"
#include "Transform.h"

void DepthScene::Start(){
	DemoScene::Start();

	SetAmbientColor(Color(0.1f));

	shader = CREATE Shader("Shaders/DepthTest.vert", "Shaders/DepthTest.frag");
	shader->AddUniform("Far", "uFar", 30.f);
	shader->AddUniform("Near", "uNear", 0.1f);
	shader->Compile();

	car_mat = CREATE Material(shader);
	Entity* camaro = GetModel("Models/Camaro/Camaro.fbx")->GetHierarchy();
	ApplyMaterial(camaro, car_mat);
	AddEntity(camaro);
	
	road_mat = CREATE Material(shader);
	Entity* road = LoadPrimitive(Model::Primitive::PLANE);
	road->GetTransform()->SetScale(15.f);
	road->GetComponent<Mesh>()->SetMaterial(road_mat);
	AddEntity(road);
}

void DepthScene::Stop(){
	delete road_mat;
	delete car_mat;
	delete shader;
	DemoScene::Stop();
}

void DepthScene::Update(float sec){
	DemoScene::Update(sec);
}