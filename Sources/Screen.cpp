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
#include "Screen.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Launcher.h"
#include "UI.h"

using namespace cross;

void Screen::Start(){
	action_down_delegate = MakeDelegate(this, &Screen::ActionDownHandle);
	action_move_delegate = MakeDelegate(this, &Screen::ActionMoveHandle);
	action_up_delegate = MakeDelegate(this, &Screen::ActionUpHandle);
	input->ActionDown += action_down_delegate;
	input->ActionMove += action_move_delegate;
	input->ActionUp += action_up_delegate;
	for(U32 i = 0; i < MAX_ACTIONS; i++){
		actionIDs[i] = false;
	}
}

void Screen::Update(float sec){
	for(UI* ui : guis){
		ui->Update(sec);
	}
}

void Screen::Stop(){
	input->ActionDown -= action_down_delegate;
	input->ActionMove -= action_move_delegate;
	input->ActionUp -= action_up_delegate;
	for(UI* ui : guis){
		delete ui;
	}
	guis.clear();
}

float Screen::GetWidth(){
	return gfx2D->GetCamera()->GetViewWidth();
}

float Screen::GetHeight(){
	return gfx2D->GetCamera()->GetViewHeight();
}

float Screen::GetScaleFactor(){
	return (float)launcher->GetTargetWidth() / GetWidth();
}

void Screen::SetBackground(const Color& c){
	glClearColor(c.R, c.G, c.B, 1.f);
}

void Screen::ActionDownHandle(Input::Action action){
	if(!OnGuiArea(action.pos)){
		actionIDs[action.id] = true;
		ActionDown(action);
	}
}

void Screen::ActionMoveHandle(Input::Action action){
	if(actionIDs[action.id]){
		ActionMove(action);
	}
}

void Screen::ActionUpHandle(Input::Action action){
	if(actionIDs[action.id]){
		actionIDs[action.id] = false;
		ActionUp(action);
	}
}

bool Screen::OnGuiArea(Vector2D pos){
	for(UI* ui : guis){
		if(ui->OnLocation(pos)){
			return true;
		}
	}
	return false;
}