#pragma once
#include "Scene.h"
#include "System.h"
#include "Math/Vector3D.h"

using namespace cross;

class CameraController;

class DemoScene : public Scene {
public:
	//Service entities will not be shown on SceneView and will be removed before scene saving
	Entity* service_root = nullptr;

	void Start() override;
	void Stop() override;
	void Update(float sec) override;

	void Save(const String& file) override;

	void ApplyMaterial(Entity* entity, Material* mat, bool depthTest = true);
	Entity* ImportModel(const String& modelFile, bool calcTangents = false);
	void DrawVector(const Vector3D& vec, const Vector3D& pos = Vector3D::Zero);

protected:
	CameraController* camera_controller = nullptr;

private:
	const String ArrowModelFile = "Models/Arrow.fbx";

	bool draw_vector = false;
	Entity* arrow = nullptr;
	Material* arrow_mat = nullptr;

	void OnOrientationChanged(System::Orientation o);
};
