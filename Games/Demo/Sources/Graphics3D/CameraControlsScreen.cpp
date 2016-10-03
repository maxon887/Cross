/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "CameraControlsScreen.h"
#include "Demo.h"
#include "Sprite.h"

void CameraControlsScreen::Start(){
	FreeCameraScene::Start();
	//UI
	arrow_released = demo->GetCommonSprite("ArrowUp.png");
	arrow_pressed = demo->GetCommonSprite("ArrowDown.png");

	//button
	float btnWidth = 300.f;
	up_btn = new Button(Vector2D(btnWidth, btnWidth));
	Sprite* cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(90.f);
	Sprite* clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(90.f);
	up_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(up_btn);
	
	left_btn = new Button(Vector2D(btnWidth/3.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(180.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(180.f);
	left_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(left_btn);

	down_btn = new Button(Vector2D(btnWidth, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(-90.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(-90.f);
	down_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(down_btn);

	right_btn = new Button(Vector2D(btnWidth + (btnWidth / 3.f) * 2.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	clonePressed = arrow_pressed->Clone();
	right_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(right_btn);

	Sprite* eye = demo->GetCommonSprite("EyeBtn.png");
	Sprite* eyePressed = demo->GetCommonSprite("EyeBtnPressed.png");
	eye_btn = new ToggleButton(eye, eyePressed);
	eye_btn->Clicked += MakeDelegate(this, &CameraControlsScreen::OnEyeClick);
	eye_btn->SetLocation(Vector2D(GetWidth() - eye_btn->GetWidth()/2.f, GetHeight() - eye_btn->GetHeight()/2.f));
	eye_btn->SetState(false);
	gui.push_back(eye_btn);
}

void CameraControlsScreen::Stop(){
	delete arrow_released;
	delete arrow_pressed;
	FreeCameraScene::Stop();
}

void CameraControlsScreen::Update(float sec){
	FreeCameraScene::Update(sec);

	if(up_btn->IsPressed()) {
		MoveForward(sec);
	}
	if(down_btn->IsPressed()) {
		MoveBackward(sec);
	}
	if(left_btn->IsPressed()) {
		MoveLeft(sec);
	}
	if(right_btn->IsPressed()) {
		MoveRight(sec);
	}

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
		return;
	}
}

void CameraControlsScreen::OnEyeClick(){
	if(eye_btn->GetState()){
		FreeCamera();
	}else{
		OrbitCamera(GetCamera()->GetPosition().Length());
	}
}