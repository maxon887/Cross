#include "MainScreen.h"
#include "Font.h"





void MainScreen::Start(){
	font = new Font("Engine/times.ttf", 10, Color::Red);
}

void MainScreen::Update(float sec){
	gfx2D->Clear();
	gfx2D->DrawText(Vector2D(-0.5f, -0.5f), "Hello world", font);
}

MainScreen::~MainScreen(){
}
