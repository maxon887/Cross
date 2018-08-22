/*	Copyright � 2015 Lukyanau Maksim

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
#include "Physics/Suppressors.h"
#include "System.h"
#include "Camera.h"
#include "Entity.h"
#include "Physics/RigidBody.h"

void Suppressors::Start(){
	ApocalypseScene::Start();
	GetCamera()->SetPosition(Vector3D(-3.2f, 2.1f, 4.4f));
	GetCamera()->GetEntity()->GetComponent<Transform>()->LookAt(Vector3D::Zero);

	Entity* camaro = GetEntity("Axel");
	//camaro->SetParent(NULL);
	camaro->GetComponent<Transform>()->SetPosition(Vector3D(0.f, 10.f, 0.f));
	//RigidBody* camaroRigid = new RigidBody(1500.f);
	//camaro->AddComponent(camaroRigid);
}

void Suppressors::Stop(){
	ApocalypseScene::Stop();
}

void Suppressors::Update(float sec){
	ApocalypseScene::Update(sec);
}