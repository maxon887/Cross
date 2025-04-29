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
#include "Log.h"
#include "System.h"

void Log::PreUpdate() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
}

void Log::Update(float sec) {
	if(ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	if(ImGui::Button("Copy")) {
		const String& logBuffer = os->GetLogBuffer();
		os->SetClipboard(logBuffer);
	}
	ImGui::SameLine();
	filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if(filter.IsActive()) {
		const char* line_start = os->GetLogBuffer();
		const char* current = line_start;
		while(*current != 0) {
			if(*current == '\n') {
				if(filter.PassFilter(line_start, current)) {
					ImGui::TextUnformatted(line_start, current);
				}
				line_start = current + 1;
			}
			current++;
		}
	} else {
		if(os->GetLogBuffer().Length() > 0) {
			ImGui::TextUnformatted(os->GetLogBuffer());
		}
	}

	if(log_size != os->GetLogBuffer().Length()) {
		scroll_to_bottom = true;
		log_size = os->GetLogBuffer().Length();
	}

	if(scroll_to_bottom) {
		ImGui::SetScrollHereY(1.0f);
	}
	scroll_to_bottom = false;
	ImGui::EndChild();
}

void Log::Clear() {
	os->GetLogBuffer().Clear();
}
