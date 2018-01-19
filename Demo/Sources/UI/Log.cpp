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

Log::Log() : View("Log")
{
	system->Logged.Connect(this, &Log::OnLogged);
}

Log::~Log() {
	system->Logged.Disconnect(this, &Log::OnLogged);
}

void Log::WillContent() {
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
}

void Log::Content(float sec) {
	if(ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if(copy) ImGui::LogToClipboard();

	if(filter.IsActive()) {
		const char* buf_begin = buffer.begin();
		const char* line = buf_begin;
		for(int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < lineoffset.Size) ? buf_begin + lineoffset[line_no] : NULL;
			if(filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	} else {
		ImGui::TextUnformatted(buffer.begin());
	}

	if(scroll_to_bottom) {
		ImGui::SetScrollHere(1.0f);
	}
	scroll_to_bottom = false;
	ImGui::EndChild();
	//ImGui::End();
}

void Log::Clear() {
	buffer.clear();
	lineoffset.clear();
}

void Log::OnLogged(const char* msg) {
	int old_size = buffer.size();
	buffer.appendf(msg);
	buffer.appendf("\n");
	for(int new_size = buffer.size(); old_size < new_size; old_size++)
		if(buffer[old_size] == '\n')
			lineoffset.push_back(old_size);
	scroll_to_bottom = true;
}