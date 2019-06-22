#include "pch.h"
#include "imguigm.h"

#include "gm/gmbind.h"
#include "gm/gmbindfuncgen.h"
#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "Imgui.h"

namespace prev {

	void ImGuiOutputTable(gmTableObject * table, gmUserArray * selectArray, bool showFunctions) {

	}

	struct gmfImGuiLib {
		GM_MEMFUNC_DECL(Begin) {
			Vec2i pos = Vec2i(ImGui::AUTOPOS);
			Vec2i dimen = Vec2i(ImGui::AUTOSIZE);

			int numParam = a_thread->GetNumParams();
			GM_CHECK_STRING_PARAM(name, 0);

			if (numParam > 1 && a_thread->Param(1).IsVec2i()) {
				GM_VEC2I_PARAM(paramPos, 1);
				pos = paramPos;
			}

			if (numParam > 2 && a_thread->Param(2).IsVec2i()) {
				GM_VEC2I_PARAM(paramDimen, 1);
				dimen = paramDimen;
			}

			ImGui::Begin(name, pos, dimen);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(End) {
			GM_CHECK_NUM_PARAMS(0);
			ImGui::End();
			return GM_OK;
		}
	};

	struct gmFunctionEntry s_gmImGuiLib[] = {
		GM_LIBFUNC_ENTRY(Begin, ImGui)
		GM_LIBFUNC_ENTRY(End, ImGui)
	};

	void gmBindImGuiLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_gmImGuiLib, GM_LIBFUNC_SIZE(s_gmImGuiLib), "Gui");
	}

}