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
#include "Stats.h"
#include "Demo.h"
#include "System.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

void Stats::WillContent() {
	ImGui::SetNextWindowSize(ImVec2(SCALED(200), SCALED(90)), ImGuiCond_FirstUseEver);
}

void Stats::Content(float sec) {
	ImGui::Text("FPS - %.1f(%.2f ms)", Debugger::Instance()->GetFPS(), Debugger::Instance()->GetUpdateTime());
	ImGui::Text("CPU Time - %.2f ms", Debugger::Instance()->GetCPUTime());
#ifdef CROSS_DEBUG
	U64 memory = MemoryManager::Instance()->AllocatedMemory();
	if(memory > 10 * 1024) {
		if(memory > 10 * 1024 * 1024) {
			ImGui::Text("Memory Used - %.2f mb", memory / (1024.f * 1024.f));
		} else {
			ImGui::Text("Memory Used - %.2f kb", memory / 1024.f);
		}
	} else {
		ImGui::Text("Memory Used - %llu b", memory);
	}
#endif // CROSS_DEBUG
}
