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
#include "View.h"
#include "System.h"

#include "ThirdParty/ImGui/imgui.h"

View::View(string name)
{
	landscape_name = name;
	portrait_name = name + " ";
}

void View::Show() {
	visible = true;
	Shown();
}

void View::Hide() {
	visible = false;
	Hidden();
}

bool View::IsVisible() const {
	return visible;
}

void View::Update(float sec) {
	if(visible) {
		const char* name = landscape_name.c_str();
		switch(system->GetDeviceOrientation()) {
		case System::Orientation::LANDSCAPE:
			name = landscape_name.c_str();
			break;
		case System::Orientation::PORTRAIT:
			name = portrait_name.c_str();
			break;
		default:
			CROSS_FAIL(false, "Unknown device orientation");
			break;
		}

		WillContent();
		ImGui::Begin(name, &visible, flags);
		Content(sec);
		ImGui::End();
		DidContent();
	}
}

const string& View::GetName() const {
	return landscape_name;
}

void View::SetWindowFlags(U32 f) {
	this->flags = f;
}