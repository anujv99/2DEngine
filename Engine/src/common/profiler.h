#pragma once

#include "graphics/linegraph.h"
#include "utils/layerstack.h"

#define PROFILER_ROOT_BEGIN { if (prev::Profiler::InstExists()) prev::Profiler::Ref().Begin(); }
#define PROFILER_ROOT_END { if (prev::Profiler::InstExists()) prev::Profiler::Ref().End(); }

#define PROFILER_BEGIN(name) { if (prev::Profiler::InstExists()) prev::Profiler::Ref().BeginEntry(name); }
#define PROFILER_END(name) { if (prev::Profiler::InstExists()) prev::Profiler::Ref().EndEntry(name); }

namespace prev {

	class Profiler : public Singleton<Profiler> {
		friend class Singleton<Profiler>;
	private:
		Profiler();
		~Profiler();
	public:
		void Begin();
		void End();
		void BeginEntry(const std::string & name);
		void EndEntry(const std::string & name);
	private:
		typedef uint32_t EntryKey;

		struct Entry {
			Entry(const std::string & name, Entry * parent) : 
				Name(name), 
				Parent(parent), 
				StartTime(0.0f), EndTime(0.0f), LastDelta(0.0f),
				Expanded(true), DrawLineGraph(false), Graph(0.0f, 30.0f) {
			}

			std::string Name;
			Entry * Parent;

			float StartTime;
			float EndTime;
			float LastDelta;
			bool Expanded;
			bool DrawLineGraph;

			std::unordered_map<EntryKey, Entry> Children;
			LineGraph Graph;
		};
		
		EntryKey GetKey(const std::string & name);
		Entry * GetChildEntry(Entry & parent, const std::string & name);
		void GuiEntry(Entry * entry, unsigned int level);
		void GuiGraph(Entry * entry);
		void GuiGraphs();
		void Gui();

		Entry m_RootEntry;
		Entry * m_ActiveEntry;
		bool m_IsPaused;
		bool m_PlotGraph;
		bool m_DrawGui;
	};

}