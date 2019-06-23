#include "pch.h"
#include "virtualmachine.h"

#include "gm/gmMachine.h"
#include "gm/gmUtilEx.h"

#include "virtualmachinelibs.h"
#include "utils/layerstack.h"
#include "imgui/imguilayer.h"
#include "graphics/window.h"

#include <imgui.h>

namespace prev {

	VirtualMachine::VirtualMachine() : 
		m_NumThreads(0), m_ThreadID(0), 
		m_DrawMs(0.0f), m_UpdateMs(0.0f), m_LastCallTime(0u),
		m_VM(nullptr),
		m_DrawFunction(nullptr), m_ClearFunction(nullptr), m_DrawManager(GM_NULL),
		m_FreezThreadAllocationGui(false), m_ShowThreadAllocationGui(false), m_ShowSettingsGui(false) {

		m_VM = new gmMachine();
		m_VM->SetAutoMemoryUsage(true);

		InitGuiSettings();
		InitGuiThreadAllocations();

		LayerStack::Ref().GetImGuiLayer()->AddGuiFunction(std::bind(&VirtualMachine::GuiSettings, this));
	}

	VirtualMachine::~VirtualMachine() {
		delete m_VM;
		m_VM = nullptr;

		m_Console.Log("Virtual Machine Destructed");
	}

	void VirtualMachine::Update() {
		HandleErrors();

		m_UpdateMs = Timer::GetTimeMs();
		m_NumThreads = m_VM->Execute((int)Timer::GetTimeMs() - m_LastCallTime);
		m_UpdateMs = Timer::GetTimeMs() - m_UpdateMs;
		m_LastCallTime = (int)Timer::GetTimeMs();
	}

	void VirtualMachine::Render() {
		if (!m_DrawManager.IsNull()) {
			m_DrawMs = Timer::GetTimeMs();
			m_VM->GetGlobals()->Set(m_VM, "g_Rendering", gmVariable(1));
			m_VM->ExecuteFunction(m_DrawFunction, 0, true, &m_DrawManager);
			m_VM->GetGlobals()->Set(m_VM, "g_Rendering", gmVariable(0));
			m_DrawMs = Timer::GetTimeMs() - m_DrawMs;
		}
	}

	void VirtualMachine::RunMain() {
		int gcWorkPerIncrement = 400;
		int gcDestructsPerIncrement = 250;
		int memUsageSoft = 1024 * 1024 * 8;
		int memUsageHard = 1024 * 1024 * 10;

		m_VM->GetGC()->SetWorkPerIncrement(gcWorkPerIncrement);
		m_VM->GetGC()->SetDestructPerIncrement(gcDestructsPerIncrement);
		m_VM->SetDesiredByteMemoryUsageSoft(memUsageSoft);
		m_VM->SetDesiredByteMemoryUsageHard(memUsageHard);

		m_VM->SetDebugMode(true);

		RegisterLibs(m_VM);
		InitGlobals();

		m_ThreadID = gmCompileStr(m_VM, "../Engine/res/scripts/gmMain.gm");

		InitDrawManager();
	}

	void VirtualMachine::HandleErrors() {
		bool firstErr = true;

		gmLog & compileLog = m_VM->GetLog();
		const char * msg = compileLog.GetEntry(firstErr);

		if (msg) {
			const char * textHeader = "[GameMonkey Run-time Error]:";
			LOG_ERROR("{}", textHeader);
		}

		while (msg) {
			LOG_ERROR("{}", msg);
			m_Console.Log(msg, false);
			msg = compileLog.GetEntry(firstErr);
		}

		compileLog.Reset();
	}

	void VirtualMachine::InitDrawManager() {
		gmVariable drawManagerKey = gmVariable(m_VM->AllocStringObject("g_DrawManager"));
		gmTableObject * drawManager = m_VM->GetGlobals()->Get(drawManagerKey).GetTableObjectSafe();
		m_DrawManager.Nullify();

		if (drawManager) {
			m_DrawManager = gmVariable(gmVariable(drawManager));

			// grab draw functions
			gmVariable drawKey = gmVariable(m_VM->AllocStringObject("Draw"));
			m_DrawFunction = drawManager->Get(drawKey).GetFunctionObjectSafe();
			gmVariable clearKey = gmVariable(m_VM->AllocStringObject("Clear"));
			m_ClearFunction = drawManager->Get(clearKey).GetFunctionObjectSafe();
		}
	}

	void VirtualMachine::InitGlobals() {
		m_VM->GetGlobals()->Set(m_VM, "g_Rendering", gmVariable(0));
	}

	void VirtualMachine::InitGuiSettings() {
		m_ShowSettingsGui = false;

		const float minVal = 0.0f;
		const float maxVal = 16.0f;
		const float width = 200;
		const float height = 150;
		const int numVals = 128;

		m_LineGraphUpdate	= new LineGraph(minVal, maxVal, Vec2i(width, height), numVals);
		m_LineGraphDraw		= new LineGraph(minVal, maxVal, Vec2i(width, height), numVals);
		m_LineGraphMemory	= new LineGraph(minVal, maxVal, Vec2i(width, height), numVals);

	}

	std::string VirtualMachine::GuiSettings() {
		m_LineGraphUpdate->PushValue(m_UpdateMs);
		m_LineGraphDraw->PushValue(m_DrawMs);
		m_LineGraphMemory->SetMaxVal((float)m_VM->GetDesiredByteMemoryUsageHard());
		m_LineGraphMemory->PushValue((float)m_VM->GetCurrentMemoryUsage());

		int workPerIncrement = m_VM->GetGC()->GetWorkPerIncrement();
		int destructPerIncrement = m_VM->GetGC()->GetDestructPerIncrement();
		int memUsageSoft = m_VM->GetDesiredByteMemoryUsageSoft();
		int memUsageHard = m_VM->GetDesiredByteMemoryUsageHard();

		ImGui::Begin("GameMonkey Settings", (bool *)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Update");
		m_LineGraphUpdate->DrawImGui();
		ImGui::Text("Draw");
		m_LineGraphDraw->DrawImGui();
		ImGui::Text("Memory");
		m_LineGraphMemory->DrawImGui();
		ImGui::ProgressBar((float)m_VM->GetCurrentMemoryUsage() / (float)m_VM->GetDesiredByteMemoryUsageHard(), Vec2(-1, 0),
			std::to_string(m_VM->GetCurrentMemoryUsage()).c_str());
		ImGui::Text("Garbage Collector");
		if (ImGui::Button("Force Full Collect")) { m_VM->CollectGarbage(true); }
		ImGui::SliderInt("Work Per Increment", &workPerIncrement, 1, 600);
		ImGui::SliderInt("Destructs Per Increment", &destructPerIncrement, 1, 600);
		ImGui::SliderInt("Memory Usage Soft", &memUsageSoft, 200000, memUsageHard);
		ImGui::SliderInt("Memory Usage Hard", &memUsageHard, memUsageSoft + 500, memUsageSoft + 200000);
		ImGui::Separator();

		int gcWarning = m_VM->GetStatsGCNumWarnings();

		ImGui::ProgressBar(gcWarning == 0 ? 0 : 200.0f / gcWarning, Vec2(-1, 0), std::to_string(gcWarning).c_str());
		ImGui::ProgressBar(100.0f / (float)m_VM->GetStatsGCNumFullCollects());
		ImGui::ProgressBar(100.0f / (float)m_VM->GetStatsGCNumIncCollects());
		ImGui::End();

		m_VM->SetDesiredByteMemoryUsageSoft(memUsageSoft);
		m_VM->SetDesiredByteMemoryUsageHard(memUsageHard);
		m_VM->GetGC()->SetWorkPerIncrement(workPerIncrement);
		m_VM->GetGC()->SetDestructPerIncrement(destructPerIncrement);

		return "GameMonkey Settings";
	}

	void VirtualMachine::InitGuiThreadAllocations() {
		m_ShowThreadAllocationGui = false;
		m_FreezThreadAllocationGui = false;
	}

	std::string VirtualMachine::GuiThreadAllocations() {
		return "";
	}

}