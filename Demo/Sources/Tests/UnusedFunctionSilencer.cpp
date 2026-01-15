#include "Tester.h"
#include "Cross.h"

#include "Scene.h"
#include "System.h"
#include "Game.h"
#include "Entity.h"
#include "File.h"
#include "Mesh.h"
#include "Sound.h"
#include "Texture.h"
#include "Math/All.h"
#include "Shaders/Shader.h"
#include "Utils/PrimitiveDrawer.h"

using namespace cross;

void NeverUsedSilencer() {
	Scene* scene = nullptr;
	scene->RemoveEntity("Some");
	System* system = nullptr;
	File* file = nullptr;
	File::PathFromFile("SomePath");
	system->SaveAssetFile(file);
	system->IsAssetFileExists("AssetFile");
	system->RequestOrientation(System::Orientation::LANDSCAPE);
	system->PromptToExit();
	Game* game = nullptr;
	game->Suspend();
	game->Resume();
	game->IsSuspended();
	Color c = Color::Purple;
	c.SetData(nullptr);
	Entity* entity = nullptr;
	entity->FindChild(0);
	Event<> event;
	event.DisconnectAll();
	Mesh* mesh = nullptr;
	mesh->GetPolyCount();
	Sound* sound = nullptr;
	sound->Resume();
	Array<int> arr;
	arr.IsInRange(2);
	String str;
	str.FindNonFirstOf("");

	Lerp(1, 2, 0.5);
	Rect rect;
	Vector2D v2;
	Vector2D::Dot(v2, v2);
	v2.GetTruncated(20);
	Vector3D v3;
	v3.GetTruncated(20.f);
	v3.IsNormalized();
	v3.SetData(nullptr);
	Vector3D::Dot(v3, v3);
	Matrix mat = Matrix::Identity;
	PointInRect(v2, rect);
	PointInCircle(v2, v2, 10.f);
	PointInTriangle(v2, v2, v2, v2);
	DistanceSq(v2, v2);
	DistanceSq(v3, v3);
	Matrix::CreateTranslation(v3);
	Matrix::CreateScale(v3);
	mat.SetTranslation(v2);
	mat.SetRotationX(90.f);
	mat.SetRotationY(90.f);
	mat.SetRotationZ(90.f);
	Quaternion quat;
	Quaternion::DotProduct(quat, quat);
	quat.GetNormalized();
	
	Shader::Uniform prop("name", "glName");
	Cubemap* cubemap = nullptr;
	prop.SetValue(cubemap);
	
	PrimitiveDrawer::DrawPoint(v2, c);
	PrimitiveDrawer::DrawRect(rect, c);
	PrimitiveDrawer::DrawLine(v2, v2, c);
	PrimitiveDrawer::DrawLine(v3, v3, c);
	
}