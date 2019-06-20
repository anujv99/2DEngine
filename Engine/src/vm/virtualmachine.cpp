#include "pch.h"
#include "virtualmachine.h"

#include "gm/gmMachine.h"
#include "gm/gmUtilEx.h"

#include "gm/gmMathLib.h"

namespace prev {

	VirtualMachine::VirtualMachine() : 
		m_NumThreads(0), m_ThreadID(0), 
		m_DrawMs(0.0f), m_UpdateMs(0.0f), m_LastCallTime(0u),
		m_VM(nullptr),
		m_DrawFunction(nullptr), m_ClearFunction(nullptr), m_DrawManager(GM_NULL) {

		m_VM = new gmMachine();
		m_VM->SetAutoMemoryUsage(false);
	}

	VirtualMachine::~VirtualMachine() {
		delete m_VM;
		m_VM = nullptr;
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

		gmBindMathLib(m_VM);
		InitGlobals();

		m_ThreadID = gmCompileStr(m_VM, "../Engine/res/scripts/gmMain.gm");

		InitDrawManager();
	}

	void VirtualMachine::HandleErrors() {
		bool firstErr = true;

		gmLog & compileLog = m_VM->GetLog();
		const char * msg = compileLog.GetEntry(firstErr);

		if (msg) {
			const char * textHeader = "\n[GameMonkey Run-time Error]:";
			LOG_ERROR("{}", textHeader);
		}

		while (msg) {
			LOG_ERROR("{}", msg);
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

}