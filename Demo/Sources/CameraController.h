#pragma once
#include "Cross.h"
#include "Component.h"
#include "Input.h"
#include "Transform.h"

using namespace cross;

class CameraController : public Component {
public:
	enum Mode {
		ORBIT,
		FREE,
		PAD
	};

	CameraController();

	bool Activate() override;
	void Deactivate() override;
	void Update(float sec) override;
	
	void LookAtTarget(const Vector3D& target, float distance = 3);

	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void MoveCloser(float ratio);	//how close you will be compared with current distance

	void LookRight(float degree);
	void LookUp(float degree);

	Mode GetCameraMode() const;
	void SetCameraMode(Mode newMode);

protected:
	Property<float> camera_speed = Property<float>(this, "CameraSpeed", 5.f);
	Property<float> focus_distance = Property<float>(this, "FocusDistance", 3.f);
	
	bool camera_active			= false;
	Mode mode		 			= Mode::ORBIT;

	float look_target_lerp		= 0.f;
	Transform destination		= Transform();
	
	Vector2D touch_position = Vector2D();
	
	void OnActionDown(Input::Action action);
	void OnActionMove(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnKeyPressed(Key key);
	void OnKeyReleased(Key key);
	void MouseWheelRoll(float delta);
};