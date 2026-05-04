#include "AutoTests.h"
#include "Demo.h"
#include "Scenes/Simple/TriangleScene.h"
#include "Scenes/Light/DirectionalLightScene.h"
#include "Scenes/Light/PointLightScene.h"
#include "Scenes/Light/SpotLightScene.h"
#include "Scenes/Light/MultiLightScene.h"
#include "Scenes/Maps/NakedScene.h"
#include "Scenes/Maps/DiffuseScene.h"
#include "Scenes/Maps/SpecularScene.h"
#include "Scenes/Maps/RoughnessScene.h"
#include "Scenes/Maps/NormalScene.h"
#include "Scenes/Misc/DepthScene.h"
#include "Scenes/Misc/TransparencyScene.h"
#include "Scenes/Misc/SkyboxScene.h"
#include "Scenes/Misc/BillboardScene.h"

AutoTests::~AutoTests() {
	if(started) {
		game->ScreenChanged.Disconnect(this, &AutoTests::OnScreenChanged);
	}
}

void AutoTests::StartTests() {
	game->ScreenChanged.Connect(this, &AutoTests::OnScreenChanged);
	started = true;
	//Testing Triangle Screen
	game->SetScreen(CREATE TriangleScene());
}

void AutoTests::Update(float sec) {

	if(next_test_time < game->GetRunTime()) {
		if (current_test == Test::TRIANGLE_SCENE) {
			const String filename = "Scenes/Cube.scn";
			CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
		} else if (current_test == Test::CUBE_SCENE) {
			const String filename = "Scenes/TexturedCube.scn";
			CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
		} else if (current_test == Test::TEXTURED_CUBE) {
			const String filename = "Scenes/Material.scn";
			CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
		} else if (current_test == Test::MATERIAL) {
			game->SetScreen(CREATE DirectionalLightScene());
		} else if (current_test == Test::DIRECTIONAL_LIGHT) {
			game->SetScreen(CREATE PointLightScene());
		} else if (current_test == Test::POINT_LIGHT) {
			game->SetScreen(CREATE SpotLightScene());
		} else if (current_test == Test::SPOT_LIGHT) {
			game->SetScreen(CREATE MultiLightScene());
		} else if (current_test == Test::MULTI_LIGHT) {
			game->SetScreen(CREATE NakedScene());
		} else if (current_test == Test::NAKED_MODEL) {
			game->SetScreen(CREATE DiffuseScene());
		} else if (current_test == Test::DIFFUSE_MODEL) {
			game->SetScreen(CREATE SpecularScene());
		} else if (current_test == Test::SPECULAR_MODEL) {
			game->SetScreen(CREATE RoughnessScene());
		} else if (current_test == Test::ROUGHNESS_MODEL) {
			game->SetScreen(CREATE NormalScene());
		} else if (current_test == Test::NORMAL_MODEL) {
			game->SetScreen(CREATE DepthScene());
		} else if (current_test == Test::DEPTH_TEST) {
			game->SetScreen(CREATE TransparencyScene());
		} else if (current_test == Test::TRANSPARENCY) {
			game->SetScreen(CREATE SkyboxScene());
		} else if (current_test == Test::SKYBOX) {
			game->SetScreen(CREATE BillboardScene());
		}
	}
}

void AutoTests::OnScreenChanged(Screen* screen) {
	if(dynamic_cast<TriangleScene*>(screen)) {
		current_test = Test::TRIANGLE_SCENE;
	} else if(screen->GetName() == "Cube") {
		current_test = Test::CUBE_SCENE;
	} else if(screen->GetName() == "TexturedCube") {
		current_test = Test::TEXTURED_CUBE;
	} else if(screen->GetName() == "Material") {
		current_test = Test::MATERIAL;
	} else if(dynamic_cast<DirectionalLightScene*>(screen)) {
		current_test = Test::DIRECTIONAL_LIGHT;
	} else if(dynamic_cast<PointLightScene*>(screen)) {
		current_test = Test::POINT_LIGHT;
	} else if(dynamic_cast<SpotLightScene*>(screen)) {
		current_test = Test::SPOT_LIGHT;
	} else if(dynamic_cast<MultiLightScene*>(screen)) {
		current_test = Test::MULTI_LIGHT;
	} else if(dynamic_cast<NakedScene*>(screen)) {
		current_test = Test::NAKED_MODEL;
	} else if(dynamic_cast<DiffuseScene*>(screen)) {
		current_test = Test::DIFFUSE_MODEL;
	} else if(dynamic_cast<SpecularScene*>(screen)) {
		current_test = Test::SPECULAR_MODEL;
	} else if(dynamic_cast<RoughnessScene*>(screen)) {
		current_test = Test::ROUGHNESS_MODEL;
	} else if(dynamic_cast<NormalScene*>(screen)) {
		current_test = Test::NORMAL_MODEL;
	} else if(dynamic_cast<DepthScene*>(screen)) {
		current_test = Test::DEPTH_TEST;
	} else if(dynamic_cast<TransparencyScene*>(screen)) {
		current_test = Test::TRANSPARENCY;
	} else if(dynamic_cast<SkyboxScene*>(screen)) {
		current_test = Test::SKYBOX;
	} else if(dynamic_cast<BillboardScene*>(screen)) {
		current_test = Test::BILLBOARD;
	}
	next_test_time = game->GetRunTime() + 0.2f;
}