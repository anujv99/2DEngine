#include "pch.h"
#include "profiler.h"

#include "timer.h"

#include <imgui.h>
#include "imgui/imguilayer.h"
#include "utils/input.h"

namespace prev {

	static const char PROFILER_ROOT_NAME[] = "[ROOT]";
	static const char PROFILER_IMGUI_WINDOW_NAME[] = "Profiler";
	static const char PROFILER_GRAPHS_IMGUI_WINDOW_NAME[] = "Profiler Graphs";

	Profiler::Profiler() : m_ActiveEntry(nullptr), m_RootEntry(PROFILER_ROOT_NAME, nullptr), m_IsPaused(false), m_PlotGraph(false), m_DrawGui(false) {
		auto imguilayer = LayerStack::Ref().GetImGuiLayer();
		if (imguilayer == nullptr) return;
		imguilayer->AddGuiFunction(std::bind(&Profiler::Gui, this));
		imguilayer->AddGuiFunction(std::bind(&Profiler::GuiGraphs, this));
	}

	Profiler::~Profiler() {

	}

	void Profiler::Begin() {
		ASSERT(m_ActiveEntry == nullptr || m_ActiveEntry->Name == PROFILER_ROOT_NAME);

		m_ActiveEntry = &m_RootEntry;
		m_ActiveEntry->StartTime = Timer::GetTimeMs();
	}

	void Profiler::End() {
		if (m_ActiveEntry->Name != PROFILER_ROOT_NAME) {
			LOG_ERROR("Forgor to call end on profiler entry : {}", m_ActiveEntry->Name);
			ASSERT(false);
		}

		m_ActiveEntry->EndTime = Timer::GetTimeMs();
		if (m_IsPaused) return;
		m_ActiveEntry->LastDelta = m_ActiveEntry->EndTime - m_ActiveEntry->StartTime;
		m_ActiveEntry->Graph.PushValue(m_ActiveEntry->LastDelta);
	}

	void Profiler::BeginEntry(const std::string & name) {
		ASSERT(m_ActiveEntry != nullptr);

		Entry * entry = GetChildEntry(*m_ActiveEntry, name);

		entry->StartTime = Timer::GetTimeMs();
		m_ActiveEntry = entry;
	}

	void Profiler::EndEntry(const std::string & name) {
		if (name != m_ActiveEntry->Name) {
			LOG_ERROR("Forgor to call end on profiler entry : {}", m_ActiveEntry->Name);
			ASSERT(false);
		}

		ASSERT(m_ActiveEntry->Parent != nullptr);

		m_ActiveEntry->EndTime = Timer::GetTimeMs();

		if (!m_IsPaused) {
			m_ActiveEntry->LastDelta = m_ActiveEntry->EndTime - m_ActiveEntry->StartTime;
			m_ActiveEntry->Graph.PushValue(m_ActiveEntry->LastDelta);
		}

		m_ActiveEntry = m_ActiveEntry->Parent;
	}

	Profiler::EntryKey Profiler::GetKey(const std::string & name) {
		return HashString(name);
	}

	Profiler::Entry * Profiler::GetChildEntry(Entry & parent, const std::string & name) {
		ASSERT(name.size() > 0);

		EntryKey key = GetKey(name);
		auto it = parent.Children.find(key);

		if (it != parent.Children.end()) {
			return &(it->second);
		}

		auto itInsert = parent.Children.insert(std::make_pair(key, Entry(name, &parent)));
		Entry * newEntry = &(itInsert.first->second);

		return newEntry;
	}

	void Profiler::Gui() {
		if (Input::Ref().IsKeyPressed(PV_KEY_F3)) {
			m_DrawGui = !m_DrawGui;
		}

		if (!m_DrawGui)
			return;

		ImGui::Begin(PROFILER_IMGUI_WINDOW_NAME, &m_DrawGui, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Help");
		ImGui::Separator();
		ImGui::Text("[-/+] Expand Tree");
		ImGui::Text("[ x ] Plot Line Graph");
		ImGui::Separator();
		ImGui::Checkbox("Pause", &m_IsPaused);
		ImGui::Checkbox("Draw Graphs", &m_PlotGraph);
		ImGui::Text("    Entry                                                        Time(ms)    ");
		ImGui::Separator();
		GuiEntry(&m_RootEntry, 0);

		ImGui::End();
	}

	void Profiler::GuiEntry(Entry * entry, unsigned int level) {
		ASSERT(entry != nullptr);

		ImGui::PushID(entry);
		std::string str = entry->Name;

		ImGui::Checkbox("", &(entry->DrawLineGraph));
		ImGui::SameLine();
		entry->Expanded = ImGui::TreeNode(str.c_str());
		ImGui::SameLine(460);
		ImGui::Text(std::to_string(entry->LastDelta).c_str());

		if (entry->Expanded) {
			for (auto & child : entry->Children) {
				GuiEntry(&child.second, level + 1);
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void Profiler::GuiGraph(Entry * entry) {
		
		if (entry->DrawLineGraph) {
			ImGui::Text(entry->Name.c_str());
			entry->Graph.DrawImGui();
		}

		for (auto & child : entry->Children) {
			GuiGraph(&child.second);
		}

	}

	void Profiler::GuiGraphs() {
		if (!m_PlotGraph || !m_DrawGui) return;

		ImGui::Begin(PROFILER_GRAPHS_IMGUI_WINDOW_NAME, (bool *)0, ImGuiWindowFlags_AlwaysAutoResize);
		GuiGraph(&m_RootEntry);
		ImGui::End();
	}

}