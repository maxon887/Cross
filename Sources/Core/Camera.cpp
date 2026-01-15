#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "System.h"
#include "Game.h"

using namespace cross;

Camera::Camera() : Component("Camera")
{ }

bool Camera::Activate() {
	UpdateProjectionMatrix();
	view_distance.ValueChanged.Connect(this, &Camera::UpdateProjectionMatrix);
	
	Scene* scene = game->GetCurrentScene();
	CROSS_RETURN(!scene->GetCamera(), false, "Current Scene already have another camera");
	scene->SetCamera(this);
	return true;
}

void Camera::Deactivate() {
	Scene* scene = game->GetCurrentScene();
	if(scene->GetCamera() == this) {
		scene->SetCamera(nullptr);
	}
}

void Camera::Update(float sec){
	RecalcView();
}

Component* Camera::Clone() const {
	return CREATE Camera(*this);
}

float Camera::GetViewDistance() const {
	return view_distance;
}

const Matrix& Camera::GetViewMatrix() const{
	return view;
}

const Matrix& Camera::GetProjectionMatrix() const {
	return projection;
}

void Camera::UpdateProjectionMatrix() {
	projection = Matrix::CreatePerspectiveProjection(45.f, os->GetAspectRatio(), 0.1f, view_distance);
}

void Camera::RecalcView(){
	view = Matrix::Identity;
	Vector3D direction = GetTransform()->GetDirection();
	view.m[2][0] = -direction.x;
	view.m[2][1] = -direction.y;
	view.m[2][2] = -direction.z;
	Vector3D right = GetTransform()->GetRight();
	view.m[0][0] = right.x;
	view.m[0][1] = right.y;
	view.m[0][2] = right.z;
	Vector3D up = GetTransform()->GetUp();
	view.m[1][0] = up.x;
	view.m[1][1] = up.y;
	view.m[1][2] = up.z;

	Vector3D position = GetPosition();
	Matrix posMatrix = Matrix::Identity;
	posMatrix.m[0][3] = -position.x;
	posMatrix.m[1][3] = -position.y;
	posMatrix.m[2][3] = -position.z;

	view = view * posMatrix;
}