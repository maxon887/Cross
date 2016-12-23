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
#include "CameraControlsScene.h"
#include "Demo.h"
#include "Sprite.h"
#include "Camera.h"

void CameraControlsScene::Start(){
	DebugScene::Start();
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -3.f));
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
	AddUI(up_btn);
	
	left_btn = new Button(Vector2D(btnWidth/3.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(180.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(180.f);
	left_btn->SetImages(cloneReleased, clonePressed);
	AddUI(left_btn);

	down_btn = new Button(Vector2D(btnWidth, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(-90.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(-90.f);
	down_btn->SetImages(cloneReleased, clonePressed);
	AddUI(down_btn);

	right_btn = new Button(Vector2D(btnWidth + (btnWidth / 3.f) * 2.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	clonePressed = arrow_pressed->Clone();
	right_btn->SetImages(cloneReleased, clonePressed);
	AddUI(right_btn);

	Sprite* eye = demo->GetCommonSprite("EyeBtn.png");
	Sprite* eyePressed = demo->GetCommonSprite("EyeBtnPressed.png");
	eye_btn = new ToggleButton(eye, eyePressed);
	eye_btn->Clicked += MakeDelegate(this, &CameraControlsScene::OnEyeClick);
	eye_btn->SetLocation(Vector2D(GetWidth() - eye_btn->GetWidth()/2.f, GetHeight() - eye_btn->GetHeight()/2.f));
	eye_btn->SetState(false);
	AddUI(eye_btn);

	OnEyeClick();
}

void CameraControlsScene::Stop(){
	delete arrow_released;
	delete arrow_pressed;
	DebugScene::Stop();
}

void CameraControlsScene::Update(float sec){
	DebugScene::Update(sec);

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
}

void CameraControlsScene::OnEyeClick(){
	LookAtCamera(!eye_btn->GetState());
}
