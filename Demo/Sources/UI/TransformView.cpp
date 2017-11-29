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
#include "TransformView.h"
#include "Entity.h"
#include "Transform.h"

#include "ThirdParty/ImGui/imgui.h"

void TransformView::Content(Transform* tranform) {
	float posVec[3];
	memcpy(posVec, tranform->GetPosition().GetData(), 3 * sizeof(float));
	if(ImGui::DragFloat3("Position", posVec, 0.1f)) {
		tranform->SetPosition(Vector3D(posVec[0], posVec[1], posVec[2]));
	}
}
