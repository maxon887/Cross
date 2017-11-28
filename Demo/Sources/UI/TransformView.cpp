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
#include "Game.h"

#include "ThirdParty/ImGui/imgui.h"

TransformView::TransformView() : View("Transform") {
	game->ScreenChanged.Connect(this, &TransformView::OnScreenChanged);
}

void TransformView::Content() {
	if(selected_entity) {
		float posVec[3];
		memcpy(posVec, selected_entity->GetTransform()->GetPosition().GetData(), 3 * sizeof(float));
		ImGui::InputFloat3("Position", posVec);
	}
}

void TransformView::OnEntitySelected(Entity* entity) {
	selected_entity = entity;
}

void TransformView::OnScreenChanged(Screen* entity) {
	selected_entity = NULL;
}