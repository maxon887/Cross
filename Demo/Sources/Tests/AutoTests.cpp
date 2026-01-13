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
	if(TriangleScreenLoaded + 0.2f < game->GetRunTime()) {
		demo->ToMain();
	}
}

void AutoTests::OnScreenChanged(Screen* screen) {
	if(dynamic_cast<TriangleScene*>(screen)) {
		TriangleScreenLoaded = game->GetRunTime();
	}
}