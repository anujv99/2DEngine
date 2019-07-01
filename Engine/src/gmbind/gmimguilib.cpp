#include "pch.h"
#include "gmimguilib.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "gm/gmArrayLib.h"
#include "gm/gmUtil.h"

#include "gmbind/gmbind.h"
#include "gmbind/gmbindfuncgen.h"
#include "vm/virtualmachine.h"

namespace prev {

	void OutputGmTable(gmTableObject * table, gmUserArray * selectArr, int level, bool showFunctions) {
		if (level >= selectArr->Size()) return;

		std::vector<gmVariable *> keys;
		gmSortTableKeys(table, keys);

		const int charsPerTab = 2;
		const int dotPadding = 56 - level * charsPerTab;

		for (size_t i = 0; i < keys.size(); ++i) {
			gmVariable & key = *keys[i];
			gmVariable val = table->Get(key);
			int getLevelVal = selectArr->GetAt(level).GetInt();
			bool selected = getLevelVal == i;

			if (!showFunctions && val.IsFunction())
				continue;

			char buffer[256];
			const char * strKey = key.AsString(&VirtualMachine::Get()->GetVM(), buffer, sizeof(buffer));

			if (val.IsTable()) {
				bool dropDownSelected = selected;
				dropDownSelected = ImGui::CollapsingHeader(strKey) != 0;

				// clicked down
				if (dropDownSelected != selected) {
					selectArr->SetAt(level, gmVariable(dropDownSelected ? (int)i : -1));

					for (int j = level + 1; j < selectArr->Size(); ++j) {
						selectArr->SetAt(j, gmVariable(-1));
					}
				}

				selected = dropDownSelected;
			} else {
				ImGui::Text(" - ");
				ImGui::SameLine();
				ImGui::Text(strKey);
			}

			// dots
			int keyLen = (int)strlen(strKey);
			int numDots = dotPadding - keyLen;
			memset(buffer, '.', numDots);
			buffer[numDots] = 0;

			ImGui::SameLine();
			ImGui::Text(buffer);
			ImGui::SameLine();
			const char * strVal = val.AsString(&VirtualMachine::Get()->GetVM(), buffer, sizeof(buffer));
			ImGui::Text(strVal);

			if (val.IsTable()) {
				gmTableObject * childTable = val.GetTableObjectSafe();

				// print table count
				char tableCountBuffer[8];
				sprintf(tableCountBuffer, "(%d)", childTable->Count());
				ImGui::SameLine();
				ImGui::Text(tableCountBuffer);

				// if table, print it
				if (selected) {
					ImGui::Indent();
					OutputGmTable(childTable, selectArr, level + 1, showFunctions);
					ImGui::Unindent();
				}
			}
		}
	}

	void ImGuiOutputTable(gmTableObject * table, gmUserArray * selectArray, bool showFunctions) {
		OutputGmTable(table, selectArray, 0, showFunctions);
	}

	void ImGuiSafeEnd() {
		if (ImGui::GetCurrentContext()->CurrentWindowStack.size() != 1 && ImGui::GetCurrentContext()->CurrentWindow) {
			ImGui::End();
		}
	}

	struct gmfImGuiLib {

		GM_MEMFUNC_DECL(Begin) {
			GM_CHECK_NUM_PARAMS_RANGE(1, 2);
			int num_params = GM_NUM_PARAMS;

			GM_CHECK_STRING_PARAM(name, 0);

			ImGuiWindowFlags winFlags = 0;

			if (num_params == 2) {
				GM_CHECK_VEC2_OR_VEC2I_PARAM(size, 1);
				ImGui::Begin(name, nullptr, size);
			} else {
				winFlags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize;
				ImGui::Begin(name, nullptr, winFlags);
			}

			return GM_OK;
		}

		GM_MEMFUNC_DECL(End) {
			ImGuiSafeEnd();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderInt) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_CHECK_INT_PARAM(val, 1);
			GM_CHECK_INT_PARAM(min, 2);
			GM_CHECK_INT_PARAM(max, 3);

			ImGui::SliderInt(name, &val, min, max);
			a_thread->PushInt(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderVec2i) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC2I_PARAM(val, 1);
			GM_CHECK_INT_PARAM(min, 2);
			GM_CHECK_INT_PARAM(max, 3);

			ImGui::SliderInt2(name, &val[0], min, max);
			a_thread->PushVec2i(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderFloat) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_CHECK_FLOAT_PARAM(val, 1);
			GM_CHECK_FLOAT_OR_INT_PARAM(min, 2);
			GM_CHECK_FLOAT_OR_INT_PARAM(max, 3);

			ImGui::SliderFloat(name, &val, min, max);
			a_thread->PushFloat(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderVec2) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC2_PARAM(val, 1);
			GM_CHECK_FLOAT_OR_INT_PARAM(min, 2);
			GM_CHECK_FLOAT_OR_INT_PARAM(max, 3);

			ImGui::SliderFloat2(name, &val[0], min, max);
			a_thread->PushVec2(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderVec3) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC3_PARAM(val, 1);
			GM_CHECK_FLOAT_OR_INT_PARAM(min, 2);
			GM_CHECK_FLOAT_OR_INT_PARAM(max, 3);

			ImGui::SliderFloat3(name, &val[0], min, max);
			a_thread->PushVec3(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(SliderVec4) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC4_PARAM(val, 1);
			GM_CHECK_FLOAT_OR_INT_PARAM(min, 2);
			GM_CHECK_FLOAT_OR_INT_PARAM(max, 3);

			ImGui::SliderFloat4(name, &val[0], min, max);
			a_thread->PushVec4(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(Color3) {
			GM_CHECK_NUM_PARAMS(2);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC3_PARAM(val, 1);

			ImGui::ColorPicker3(name, &val[0]);
			a_thread->PushVec3(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(Color4) {
			GM_CHECK_NUM_PARAMS(2);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC4_PARAM(val, 1);

			ImGui::ColorPicker4(name, &val[0]);
			a_thread->PushVec4(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(ColorEdit3) {
			GM_CHECK_NUM_PARAMS(2);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC3_PARAM(val, 1);

			ImGui::ColorEdit3(name, &val[0]);
			a_thread->PushVec3(val);

			return GM_OK;
		}

		GM_MEMFUNC_DECL(ColorEdit4) {
			GM_CHECK_NUM_PARAMS(2);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_VEC4_PARAM(val, 1);

			ImGui::ColorEdit4(name, &val[0]);
			a_thread->PushVec4(val);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(TextInput) {
			GM_CHECK_NUM_PARAMS(2);
			GM_CHECK_STRING_PARAM(name, 0);
			GM_CHECK_STRING_PARAM(buff, 1);

			std::string buffer(buff);
			ImGui::InputText(name, &buffer);
			a_thread->PushNewString(buffer.c_str(), (int)buffer.size());

			return GM_OK;
		}

		GM_MEMFUNC_DECL(Text) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_STRING_PARAM(text, 0);

			ImGui::Text(text);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(Separator) {
			GM_CHECK_NUM_PARAMS(0);
			ImGui::Separator();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(Button) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_STRING_PARAM(name, 0);
			a_thread->PushInt(ImGui::Button(name));
			return GM_OK;
		}

	};

	static gmFunctionEntry s_ImGuiLib[] = {
		GM_LIBFUNC_ENTRY(Begin, ImGui)
		GM_LIBFUNC_ENTRY(End, ImGui)
		GM_LIBFUNC_ENTRY(SliderInt, ImGui)
		GM_LIBFUNC_ENTRY(SliderVec2i, ImGui)
		GM_LIBFUNC_ENTRY(SliderFloat, ImGui)
		GM_LIBFUNC_ENTRY(SliderVec2, ImGui)
		GM_LIBFUNC_ENTRY(SliderVec3, ImGui)
		GM_LIBFUNC_ENTRY(SliderVec4, ImGui)
		GM_LIBFUNC_ENTRY(Color3, ImGui)
		GM_LIBFUNC_ENTRY(Color4, ImGui)
		GM_LIBFUNC_ENTRY(ColorEdit3, ImGui)
		GM_LIBFUNC_ENTRY(ColorEdit4, ImGui)
		GM_LIBFUNC_ENTRY(TextInput, ImGui)
		GM_LIBFUNC_ENTRY(Text, ImGui)
		GM_LIBFUNC_ENTRY(Separator, ImGui)
		GM_LIBFUNC_ENTRY(Button, ImGui)

	};

	void gmBindImGuiLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_ImGuiLib, GM_LIBFUNC_SIZE(s_ImGuiLib), "Gui");
	}

}
