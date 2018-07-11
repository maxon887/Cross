#include "TransformGizmo.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Model.h"
#include "Entity.h"
#include "Mesh.h"
#include "Transform.h"
#include "Game.h"
#include "Scene.h"
#include "Camera.h"

TransformGizmo::TransformGizmo() {
	shader = new Shader();
	shader->Load("Engine/Shaders/Simple.sha");
	shader->Compile();

	red_mat = new Material(shader);
	red_mat->SetPropertyValue("Color", Color::Red);
	green_mat = new Material(shader);
	green_mat->SetPropertyValue("Color", Color::Green);
	blue_mat = new Material(shader);
	blue_mat->SetPropertyValue("Color", Color::Blue);

	arrow = new Model();
	arrow->Load("Editor/Models/Arrow.obj");

	Entity* xArrow = arrow->GetHierarchy();
	xArrow->SetName("XArrow");
	xArrow->GetComponent<Mesh>()->SetMaterial(red_mat);
	xArrow->GetComponent<Transform>()->SetRotate(Vector3D(0.f, 0.f, 1.f), 90.f);

	Entity* yArrow = arrow->GetHierarchy();
	yArrow->SetName("YArrow");
	yArrow->GetComponent<Mesh>()->SetMaterial(green_mat);

	Entity* zArrow = arrow->GetHierarchy();
	zArrow->SetName("ZArrow");
	zArrow->GetComponent<Mesh>()->SetMaterial(blue_mat);
	zArrow->GetComponent<Transform>()->SetRotate(Vector3D(1.f, 0.f, 0.f), 90.f);

	entity = new Entity("TransformGizmo");
	entity->AddComponent(new Transform());
	entity->AddChild(xArrow);
	entity->AddChild(yArrow);
	entity->AddChild(zArrow);
}

TransformGizmo::~TransformGizmo() {
	delete entity;
	delete arrow;
	delete green_mat;
	delete blue_mat;
	delete red_mat;
	delete shader;
}

void TransformGizmo::Update(float sec) {
	SAFE(glClear(GL_DEPTH_BUFFER_BIT));
	entity->Update(sec);

	Camera* camera = game->GetCurrentScene()->GetCamera();
	Vector3D camPos = camera->GetEntity()->GetComponent<Transform>()->GetPosition();
	Vector3D gizmoPos = entity->GetComponent<Transform>()->GetPosition();
	Vector3D diff = camPos - gizmoPos;
	float scale = diff.Length() / ScaleFactor;
	entity->GetComponent<Transform>()->SetScale(scale);
}