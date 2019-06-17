#include "pch.h"
#include "profiler.h"

#include "imgui/imgui.h"

#include "timer.h"

namespace prev {

	static const char PROFILER_ROOT_NAME[] = "[ROOT]";
	static const char PROFILER_IMGUI_WINDOW_NAME[] = "Profiler";

	Profiler::Profiler() : m_ActiveEntry(nullptr), m_RootEntry(PROFILER_ROOT_NAME, nullptr), m_IsPaused(false), m_PlotGraph(false) {}

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

	void Profiler::PushGUILayer() {
		LayerStack::Ref().PushLayer(new ProfilerGuiLayer());
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
		ImGui::Begin(PROFILER_IMGUI_WINDOW_NAME);

		ImGui::Header("Help");
		ImGui::Print("[-/+] Expand Tree");
		ImGui::Print("[ x ] Plot Line Graph");
		ImGui::Separator();
		ImGui::CheckBox("Pause", m_IsPaused);
		ImGui::CheckBox("Draw Graphs", m_PlotGraph);
		const char str[] = "Entry                                                    ";
		ImGui::Header("       Entry                                                    Time(ms)");
		GuiEntry(&m_RootEntry, 0);
		ImGui::End();

		if (!m_PlotGraph) return;
		ImGui::Begin("Profiler Graphs");
		GuiGraph(&m_RootEntry);
		ImGui::End();
	}

	void Profiler::GuiEntry(Entry * entry, unsigned int level) {
		ASSERT(entry != nullptr);

		std::string str;

		ImGui::DropDown("", entry->Expanded);
		ImGui::SameLine();
		ImGui::CheckBox("", entry->DrawLineGraph);
		ImGui::SameLine();

		for (unsigned int i = 0; i < level; i++) {
			str += "   ";
		}
		str += entry->Name;

		auto index = str.size();

		str.resize(59);
		std::fill(str.begin() + index, str.begin() + 58, '.');

		str += std::to_string(entry->LastDelta);

		ImGui::Print(str);

		if (entry->Expanded)
			for (auto & child : entry->Children) {
				GuiEntry(&child.second, level + 1);
			}
	}

	void Profiler::GuiGraph(Entry * entry) {
		if (entry->DrawLineGraph) {
			ImGui::Print(entry->Name);
			ImGui::LineGraph(entry->Graph);
		}

		for (auto & child : entry->Children) {
			GuiGraph(&child.second);
		}
	}

	std::string Profiler::ProfilerGuiLayer::OnImGuiUpdate() {
		Profiler::Ref().Gui();
		return PROFILER_IMGUI_WINDOW_NAME;
	}

}