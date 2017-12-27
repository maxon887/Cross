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
#include "Stats.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

void Stats::Content(float sec) {
	ImGui::Text("%.1f FPS(%.2f ms/frame)", Debugger::Instance()->GetFPS(), Debugger::Instance()->GetUpdateTime());
	ImGui::Text("CPU Time - %.2f ms", Debugger::Instance()->GetCPUTime());
}