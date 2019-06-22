#pragma once

#include "gm/gmVariable.h"
#include "virtualconsole.h"
#include "graphics/linegraph.h"

namespace prev {

	class VirtualMachine : public Singleton<VirtualMachine> {
		friend Singleton<VirtualMachine>;
	private:
		VirtualMachine();
		~VirtualMachine();
	public:
		void Update();
		void Render();
		void RunMain();

		gmMachine & GetVM() { return *m_VM; }
	private:
		void HandleErrors();

		void InitDrawManager();
		void InitGlobals();

		void InitGuiSettings();
		std::string GuiSettings();

		void InitGuiThreadAllocations();
		std::string GuiThreadAllocations();
	private:
		int m_NumThreads;
		int m_ThreadID;

		gmMachine * m_VM;

		gmFunctionObject * m_DrawFunction;
		gmFunctionObject * m_ClearFunction;
		gmVariable m_DrawManager;

		float m_UpdateMs;
		float m_DrawMs;
		unsigned int m_LastCallTime;

		VirtualConsole m_Console;

		struct ThreadAllocationItem {
			int Allocation;
			int EraseCountdown;
		};

		std::unordered_map<gmFunctionObject *, ThreadAllocationItem> m_ThreadAllocationHistory;
		bool m_FreezThreadAllocationGui;
		bool m_ShowThreadAllocationGui;
		bool m_ShowSettingsGui;

		StrongHandle<LineGraph> m_LineGraphUpdate;
		StrongHandle<LineGraph> m_LineGraphDraw;
		StrongHandle<LineGraph> m_LineGraphMemory;
	};

}
