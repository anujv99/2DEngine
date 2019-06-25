#include "pch.h"
#include "vmexceptionhandler.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "virtualmachine.h"

#include "gm/gmMachine.h"
#include "gmbind/gmimguilib.h"

namespace prev {

	void OnGmException() {
		if (ImGui::GetCurrentContext()->CurrentWindowStack.size() != 1 && ImGui::GetCurrentContext()->CurrentWindow) {
			gmThread * thr = VirtualMachine::Ref().GetVM().GetThread(VirtualMachine::Ref().GetConsole().GetLastCmdThread());
			if (thr == nullptr)
				ImGuiSafeEnd();
		}
	}

}