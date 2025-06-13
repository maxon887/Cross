#include "AnimatedCameraController.h"
#include "Game.h"

AnimatedCameraController::AnimatedCameraController() : Component("AnimatedCameraController") { }

void AnimatedCameraController::Update(float sec) {
	float runTime = game->GetRunTime();
	float coef = (sin(runTime / 3.f) + 1.f) / 2.f;
	Vector3D newPos = Lerp(StartPos, EndPos, coef);
	Quaternion newRotate = Lerp(StartRotate, EndRotate, coef);
	Transform* transform = GetTransform();
	transform->SetPosition(newPos);
	transform->SetRotate(newRotate);
}
