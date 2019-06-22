#include "pch.h"
#include "virtualconsole.h"
#include "graphics/window.h"
#include "imgui/Imgui.h"
#include "utils/input.h"
#include "gm/gmMachine.h"
#include "virtualmachine.h"
#include "gm/gmThread.h"

namespace prev {

	static const float CONSOLE_WIDTH = 0.4f;
	static const float CONSOLE_HEIGHT = 0.4f;
	static const int CONSOLE_PADDING = 20;
	static const int CONSOELE_INPUT_PADDING = 30;

	static const int MAX_OLD_COMMANDS = 16;
	static const char * LOG_FILE = "log.txt";

	VirtualConsole::VirtualConsole() {
		m_CmdIndex = 0;
		m_Text.reserve(1024 * 1024);

		LOG_TRACE("Virtual Console Initialized");
	}

	VirtualConsole::~VirtualConsole() {

	}

	void VirtualConsole::Log(const std::string & text, bool newLine /*= true*/) {
		if (text.empty()) return;

		LOG_TRACE("[GM] {}", text);

		m_Text += text;
		if (newLine) m_Text += "\n";

		ScrollToBottom();
	}

	void VirtualConsole::Enable(bool enable) {

	}

	void VirtualConsole::ClearText() {
		m_Text.clear();
	}

	void VirtualConsole::ScrollToBottom() {

	}

	void VirtualConsole::RunCommand(const std::string & cmd) {
		if (!cmd.empty()) {
			// remove command if reached limit
			if (m_OldCmds.size() > MAX_OLD_COMMANDS) m_OldCmds.pop_front();
			m_OldCmds.push_back(cmd);
		}
		m_CmdIndex = m_OldCmds.size();

		gmMachine * machine = &VirtualMachine::Get()->GetVM();
		ASSERT(machine);

		// add semicolon at end just in case
		char buffer[128];
		sprintf(buffer, "%s;", cmd.c_str());

		int threadId = GM_INVALID_THREAD;
		int errors = machine->ExecuteString( buffer, &threadId, false );
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

	std::string VirtualConsole::Gui() {
		if (!m_Enabled) return "";

		const Vec2i windowDimen = Window::Ref().GetDisplayMode().GetWindowSize();
		const int width = (int)windowDimen.x * CONSOLE_WIDTH;
		const int height = (int)windowDimen.y * CONSOLE_HEIGHT;

		ImGui::Begin("Console", Vec2i(windowDimen.x / 2 - width / 2, windowDimen.y - CONSOLE_PADDING), Vec2i(width, height));
		ImGui::PrintParagraph(m_Text);
		ImGui::Print(">");
		ImGui::SameLine();

		if (ImGui::TextInput("", m_Command, width - CONSOELE_INPUT_PADDING) == 1) {
			if (m_Command == "clear") {
				m_Text.clear();
				m_Command = "";
			} else {
				Log("> " + m_Command);
				RunCommand(m_Command);

				m_Command = "";
			}
		}

		if (ImGui::IsWindowActive()) {
			int deltaIndex = 0;

			if (!m_OldCmds.empty()) {
				if (Input::Get()->IsKeyPressed(0x26)) deltaIndex = -1;
				if (Input::Get()->IsKeyPressed(0x28)) deltaIndex = +1;
			}

			if (deltaIndex != 0) {
				m_CmdIndex = Clamp(m_CmdIndex + deltaIndex, 0, (int)m_OldCmds.size() - 1);
				m_Command = m_OldCmds[m_CmdIndex];
			}
		}

		ImGui::End();

		return "Console";
	}

}