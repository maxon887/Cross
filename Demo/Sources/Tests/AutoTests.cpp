#include "AutoTests.h"
#include "Demo.h"
#include "Scenes/Simple/TriangleScene.h"

void AutoTests::StartTests() {
	TestsActive = true;
	game->ScreenChanged.Connect(this, &AutoTests::OnScreenChanged);
	//Testing Triangle Screen
	game->SetScreen(CREATE TriangleScene());
}

void AutoTests::Update(float sec) {
	if(!TestsActive) {
		return;
	}
	if(TriangleSceneTime < game->GetRunTime() && TriangleSceneLoaded) {
		TriangleSceneLoaded = false;
		const String filename = "Scenes/Cube.scn";
		CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
	}
}

void AutoTests::OnScreenChanged(Screen* screen) {
	if(dynamic_cast<TriangleScene*>(screen)) {
		TriangleSceneLoaded = true;
		TriangleSceneTime = game->GetRunTime() + 0.2f;
	}
}