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
#include "PhysicsScreen.h"
#include "Physics/AABoxCollider.h"

void PhysicsScreen::Start() {
	CameraControlsScene::Start();
	AABoxCollider aabc1(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f));
	AABoxCollider aabc2(Vector3D(1.0f, 1.0f, 1.0f), Vector3D(2.0f, 2.0f, 2.0f));
	AABoxCollider aabc3(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(2.0f, 1.0f, 1.0f));
	AABoxCollider aabc4(Vector3D(0.0f, 0.0f, -2.0), Vector3D(1.0f, 1.0f, -1.0f));
	AABoxCollider aabc5(Vector3D(0.0f, 0.5f, 0.0f), Vector3D(1.0f, 1.5f, 1.0f));

	Collision collision2 = aabc1.OnCollision(&aabc2);
	Collision collision3 = aabc1.OnCollision(&aabc3);
	Collision collision4 = aabc1.OnCollision(&aabc4);
	Collision collision5 = aabc1.OnCollision(&aabc5);
}

void PhysicsScreen::Stop() {

	CameraControlsScene::Stop();
}

void PhysicsScreen::Update(float sec) {
	CameraControlsScene::Update(sec);
}