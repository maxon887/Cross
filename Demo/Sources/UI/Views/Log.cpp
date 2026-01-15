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
