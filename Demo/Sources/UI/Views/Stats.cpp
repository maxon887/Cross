#include "Stats.h"
#include "Demo.h"
#include "System.h"
#include "Utils/Debugger.h"

#include "ThirdParty/ImGui/imgui.h"

void Stats::PreUpdate() {
	ImGui::SetNextWindowSize(ImVec2(SCALED(200), SCALED(90)), ImGuiCond_FirstUseEver);
}

void Stats::Update(float sec) {
	ImGui::Text("FPS - %.1f(%.2f ms)", Debugger::Instance()->GetFPS(), Debugger::Instance()->GetUpdateTime());
	ImGui::Text("CPU Time - %.2f ms", Debugger::Instance()->GetCPUTime());
#ifdef CROSS_MEMORY_PROFILE
	U64 memory = MemoryManager::Instance()->AllocatedMemory();
	ImGui::TextUnformatted("Memory Used - " + Demo::GetCompactSize(memory));
#endif
}
