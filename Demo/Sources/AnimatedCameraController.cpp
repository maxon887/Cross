#include "AnimatedCameraController.h"
#include "Game.h"

AnimatedCameraController::AnimatedCameraController() : Component("AnimatedCameraController") { }

bool AnimatedCameraController::Initialize() {
//	StartTransform.SetPosition(Vector3D(-3.f, 0.7f, -3.f));
//	StartTransform.SetRotate(Vector3D(-0.25f, -1.f, 0), 33.f);
//	EndTransform.SetPosition(Vector3D(1.2f, 1.f, -2.2f));
//	StartTransform.SetRotate(Vector3D(-0.25f, -1.f, 0), 33.f);
	return true;
}

void AnimatedCameraController::Update(float sec) {
	float runTime = game->GetRunTime();
	float coef = (sin(runTime / 3.f) + 1.f) / 2.f;
	Vector3D newPos = Lerp(StartPos, EndPos, coef);
	Quaternion newRotate = Lerp(StartRotate, EndRotate, coef);
	Transform* transform = GetTransform();
	transform->SetPosition(newPos);
	transform->SetRotate(newRotate);
}
