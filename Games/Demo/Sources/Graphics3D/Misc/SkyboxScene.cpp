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
#include "SkyboxScene.h"

void SkyboxScene::Start(){
	skybox = new Skybox("gfx3D/Skybox/miramar_rt.png",
						"gfx3D/Skybox/miramar_lf.png",
						"gfx3D/Skybox/miramar_up.png",
						"gfx3D/Skybox/miramar_dn.png",
						"gfx3D/Skybox/miramar_bk.png",
						"gfx3D/Skybox/miramar_ft.png");
}

void SkyboxScene::Stop(){

}

void SkyboxScene::Update(float sec){
	skybox->Draw();
}