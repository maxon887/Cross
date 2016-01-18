#include "RedScreenGame.h"
#include "MainScreen.h"




RedScreenGame::RedScreenGame(Launcher* launcher) : Game(launcher) {}

void RedScreenGame::Start(){
	//Game::SetScreen(GetStartScreen());
	debuger->EnableScreenDebug();
}

Screen* RedScreenGame::GetStartScreen() {
	return new MainScreen();
}