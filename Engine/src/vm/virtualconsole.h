#pragma once

namespace prev {

	class VirtualConsole {
		friend class VirtualMachine;
	public:
		VirtualConsole();
		~VirtualConsole();

		void Log(const std::string & text, bool newLine = true);

		bool IsEnabled() const { return m_Enabled; }
		void Enable(bool enable);

		int GetLastCmdThread() const { return m_LastCmdThread; }
		void ClearText();
	private:
		void RunCommand(const std::string & cmd);
		void Gui();
	private:
		bool m_Enabled;
		bool m_QueueScrollToBottom;

		int m_LastCmdThread;

		std::string m_Text;
		std::string m_Command;

		std::deque<std::string> m_OldCmds;
		int m_CmdIndex;
	};

}
