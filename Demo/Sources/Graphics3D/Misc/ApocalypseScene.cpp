/*	Copyright © 2018 Maksim Lukyanov

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
#include "ApocalypseScene.h"

void ApocalypseScene::Start(){
	DemoScene::Start();
	SetAmbientColor(Color(0.1f));
	Load("Apocalypse.scn");
}

void ApocalypseScene::Stop(){
	DemoScene::Stop();
}

void ApocalypseScene::Update(float sec){
	DemoScene::Update(sec);
}