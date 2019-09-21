#include "pch.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "virtualconsole.h"
#include "graphics/window.h"
#include "utils/input.h"
#include "gm/gmMachine.h"
#include "virtualmachine.h"
#include "gm/gmThread.h"
#include "utils/layerstack.h"

#include "virtualconsole.inl"

static void GM_CDECL printCallback(gmMachine * a_machine, const char * str) {
	vmConsole.AddLog(str);
}

namespace prev {

	static const float CONSOLE_WIDTH = 0.4f;
	static const float CONSOLE_HEIGHT = 0.4f;
	static const int CONSOLE_PADDING = 20;
	static const int CONSOELE_INPUT_PADDING = 30;

	static const int MAX_OLD_COMMANDS = 16;
	static const char * LOG_FILE = "log.txt";

	VirtualConsole::VirtualConsole() : m_Enabled(false) {
		m_CmdIndex = 0;
		m_Text.reserve(1024 * 1024);

		ImGuiLayer * imlayer = LayerStack::Ref().GetImGuiLayer();
		if (imlayer != nullptr)
			imlayer->AddGuiFunction(std::bind(&VirtualConsole::Gui, this));

		vmConsole.CommandCallbackFunction = [this](const char * command) -> void {
			this->RunCommand(command);
		};

		gmMachine::s_printCallback = printCallback;

		LOG_TRACE("Virtual Console Initialized");
	}

	VirtualConsole::~VirtualConsole() {

	}

	void VirtualConsole::Log(const std::string & text, bool newLine /*= true*/) {
		if (text.empty()) return;
		vmConsole.AddLog(text.c_str());
	}

	void VirtualConsole::Enable(bool enable) {

	}

	void VirtualConsole::ClearText() {
		m_Text.clear();
	}

	void VirtualConsole::RunCommand(const std::string & cmd) {
		if (!cmd.empty()) {
			// remove command if reached limit
			if (m_OldCmds.size() > MAX_OLD_COMMANDS) m_OldCmds.pop_front();
			m_OldCmds.push_back(cmd);
		}
		m_CmdIndex = (int)m_OldCmds.size();

		gmMachine * machine = &VirtualMachine::Get()->GetVM();
		ASSERT(machine);

		// add semicolon at end just in case
		char buffer[128];
		sprintf(buffer, "%s;", cmd.c_str());

		int threadId = GM_INVALID_THREAD;
		//machine->ExecuteString(buffer, &threadId, false);

		if (threadId != GM_INVALID_THREAD) {
			gmThread * thread = machine->GetThread(threadId);

			if (thread) {
				m_LastCmdThread = threadId;

				// execute command
				gmVariable returnVal = gmVariable(GM_NULL);
				gmThread::State state = thread->Sys_Execute(&returnVal);

				// failed
				if (state == gmThread::EXCEPTION) {
					Log("Error: Command Failed");
					m_LastCmdThread = GM_INVALID_THREAD;
					machine->GetLog().Reset();
				}

				// successfully ran
				else if (state == gmThread::KILLED) {
					if (!returnVal.IsNull()) {
						Log(returnVal.AsStringWithType(machine, buffer, sizeof(buffer)));
					}
				}
			}
		}
	}

	void VirtualConsole::Gui() {
		if (Input::Ref().IsKeyPressed(PV_KEY_F2)) {
			m_Enabled = !m_Enabled;
		}

		if (!m_Enabled)
			return;

		vmConsole.Draw("Console", &m_Enabled);
	}

}