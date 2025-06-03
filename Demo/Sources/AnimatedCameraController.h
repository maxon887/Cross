#pragma once

#include "Component.h"
#include "Transform.h"

using namespace cross;

class AnimatedCameraController : public Component {
public:
	AnimatedCameraController();

	bool Initialize() override;
	void Update(float sec) override;

private:
	Property<Vector3D> StartPos = Property<Vector3D>(this, "StartPos");
	Property<Vector3D> EndPos = Property<Vector3D>(this, "EndPos");
	Property<Quaternion> StartRotate = Property<Quaternion>(this, "StartRotate");
	Property<Quaternion> EndRotate = Property<Quaternion>(this, "EndRotate");
};