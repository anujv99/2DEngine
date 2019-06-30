#include "pch.h"
#include "gminputlib.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "gm/gmArrayLib.h"
#include "gm/gmVariable.h"
#include "gm/gmUtil.h"

#include "gmbind.h"
#include "gmbindfuncgen.h"

#include "utils/input.h"
#include "math/vecconversion.h"

namespace prev {

	void InitInputKeyCodes(gmMachine * a_machine, gmTableObject * table);

	struct gmfInputLib {

		GM_MEMFUNC_DECL(GetMousePos) {
			GM_CHECK_NUM_PARAMS(0);
			Vec2 pos = Input::Get()->GetMousePosition();
			a_thread->PushVec2(pos);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetMouseDelta) {
			GM_CHECK_NUM_PARAMS(0);
			Vec2 pos = Input::Get()->GetMouseDeltaPosition();
			a_thread->PushVec2(pos);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetMouseScrollDelta) {
			GM_CHECK_NUM_PARAMS(0);
			Vec2 pos = ToVec2(Input::Get()->GetMouseScrollDelta());
			a_thread->PushVec2(pos);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsMouseDown) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(buttonCode, 0);
			a_thread->PushInt((int)Input::Ref().IsMouseButtonDown(buttonCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsMousePressed) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(buttonCode, 0);
			a_thread->PushInt((int)Input::Ref().IsMouseButtonPressed(buttonCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsMouseUp) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(buttonCode, 0);
			a_thread->PushInt((int)Input::Ref().IsMouseButtonUp(buttonCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsMouseReleased) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(buttonCode, 0);
			a_thread->PushInt((int)Input::Ref().IsMouseButtonReleased(buttonCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsKeyDown) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(keyCode, 0);
			a_thread->PushInt((int)Input::Ref().IsKeyDown(keyCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsKeyPressed) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(keyCode, 0);
			a_thread->PushInt((int)Input::Ref().IsKeyPressed(keyCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsKeyUp) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(keyCode, 0);
			a_thread->PushInt((int)Input::Ref().IsKeyUp(keyCode));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(IsKeyReleased) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(keyCode, 0);
			a_thread->PushInt((int)Input::Ref().IsKeyReleased(keyCode));
			return GM_OK;
		}

	};

	static gmFunctionEntry s_gmInputLib[] = {
		GM_LIBFUNC_ENTRY(GetMousePos, Input)
		GM_LIBFUNC_ENTRY(GetMouseDelta, Input)
		GM_LIBFUNC_ENTRY(GetMouseScrollDelta, Input)
		GM_LIBFUNC_ENTRY(IsMouseDown, Input)
		GM_LIBFUNC_ENTRY(IsMousePressed, Input)
		GM_LIBFUNC_ENTRY(IsMouseUp, Input)
		GM_LIBFUNC_ENTRY(IsMouseReleased, Input)
		GM_LIBFUNC_ENTRY(IsKeyDown, Input)
		GM_LIBFUNC_ENTRY(IsKeyPressed, Input)
		GM_LIBFUNC_ENTRY(IsKeyUp, Input)
		GM_LIBFUNC_ENTRY(IsKeyReleased, Input)
	};

	void gmBindInputLib(gmMachine * a_machine) {
		gmTableObject * table = a_machine->AllocTableObject();

		a_machine->GetGlobals()->Set(a_machine, "Input", gmVariable(GM_TABLE, (gmptr)table));
		a_machine->RegisterLibrary(s_gmInputLib, GM_LIBFUNC_SIZE(s_gmInputLib), "Input", false);

		InitInputKeyCodes(a_machine, table);
	}

	void InitInputKeyCodes(gmMachine * a_machine, gmTableObject * table) {
		table->Set(a_machine, "KEY_A", gmVariable(PV_KEY_A));
		table->Set(a_machine, "KEY_BACKSPACE", gmVariable(PV_KEY_BACKSPACE));
		table->Set(a_machine, "KEY_TAB", gmVariable(PV_KEY_TAB));
		table->Set(a_machine, "KEY_ENTER", gmVariable(PV_KEY_ENTER));
		table->Set(a_machine, "KEY_CAPSLOCK", gmVariable(PV_KEY_CAPSLOCK));
		table->Set(a_machine, "KEY_ESCAPE", gmVariable(PV_KEY_ESCAPE));
		table->Set(a_machine, "KEY_SPACE", gmVariable(PV_KEY_SPACE));
		table->Set(a_machine, "KEY_PAGEUP", gmVariable(PV_KEY_PAGEUP));
		table->Set(a_machine, "KEY_PAGEDOWN", gmVariable(PV_KEY_PAGEDOWN));
		table->Set(a_machine, "KEY_END", gmVariable(PV_KEY_END));
		table->Set(a_machine, "KEY_HOME", gmVariable(PV_KEY_HOME));
		table->Set(a_machine, "KEY_LEFTARROW", gmVariable(PV_KEY_LEFTARROW));
		table->Set(a_machine, "KEY_UPARROW", gmVariable(PV_KEY_UPARROW));
		table->Set(a_machine, "KEY_RIGHTARROW", gmVariable(PV_KEY_RIGHTARROW));
		table->Set(a_machine, "KEY_DOWNARROW", gmVariable(PV_KEY_DOWNARROW));
		table->Set(a_machine, "KEY_INSERT", gmVariable(PV_KEY_INSERT));
		table->Set(a_machine, "KEY_DELETE", gmVariable(PV_KEY_DELETE));
		table->Set(a_machine, "KEY_0", gmVariable(PV_KEY_0));
		table->Set(a_machine, "KEY_1", gmVariable(PV_KEY_1));
		table->Set(a_machine, "KEY_2", gmVariable(PV_KEY_2));
		table->Set(a_machine, "KEY_3", gmVariable(PV_KEY_3));
		table->Set(a_machine, "KEY_4", gmVariable(PV_KEY_4));
		table->Set(a_machine, "KEY_5", gmVariable(PV_KEY_5));
		table->Set(a_machine, "KEY_6", gmVariable(PV_KEY_6));
		table->Set(a_machine, "KEY_7", gmVariable(PV_KEY_7));
		table->Set(a_machine, "KEY_8", gmVariable(PV_KEY_8));
		table->Set(a_machine, "KEY_9", gmVariable(PV_KEY_9));
		table->Set(a_machine, "KEY_A", gmVariable(PV_KEY_A));
		table->Set(a_machine, "KEY_B", gmVariable(PV_KEY_B));
		table->Set(a_machine, "KEY_C", gmVariable(PV_KEY_C));
		table->Set(a_machine, "KEY_D", gmVariable(PV_KEY_D));
		table->Set(a_machine, "KEY_E", gmVariable(PV_KEY_E));
		table->Set(a_machine, "KEY_F", gmVariable(PV_KEY_F));
		table->Set(a_machine, "KEY_G", gmVariable(PV_KEY_G));
		table->Set(a_machine, "KEY_H", gmVariable(PV_KEY_H));
		table->Set(a_machine, "KEY_I", gmVariable(PV_KEY_I));
		table->Set(a_machine, "KEY_J", gmVariable(PV_KEY_J));
		table->Set(a_machine, "KEY_K", gmVariable(PV_KEY_K));
		table->Set(a_machine, "KEY_L", gmVariable(PV_KEY_L));
		table->Set(a_machine, "KEY_M", gmVariable(PV_KEY_M));
		table->Set(a_machine, "KEY_N", gmVariable(PV_KEY_N));
		table->Set(a_machine, "KEY_O", gmVariable(PV_KEY_O));
		table->Set(a_machine, "KEY_P", gmVariable(PV_KEY_P));
		table->Set(a_machine, "KEY_Q", gmVariable(PV_KEY_Q));
		table->Set(a_machine, "KEY_R", gmVariable(PV_KEY_R));
		table->Set(a_machine, "KEY_S", gmVariable(PV_KEY_S));
		table->Set(a_machine, "KEY_T", gmVariable(PV_KEY_T));
		table->Set(a_machine, "KEY_U", gmVariable(PV_KEY_U));
		table->Set(a_machine, "KEY_V", gmVariable(PV_KEY_V));
		table->Set(a_machine, "KEY_W", gmVariable(PV_KEY_W));
		table->Set(a_machine, "KEY_X", gmVariable(PV_KEY_X));
		table->Set(a_machine, "KEY_Y", gmVariable(PV_KEY_Y));
		table->Set(a_machine, "KEY_Z", gmVariable(PV_KEY_Z));
		table->Set(a_machine, "KEY_NUM0", gmVariable(PV_KEY_NUM0));
		table->Set(a_machine, "KEY_NUM1", gmVariable(PV_KEY_NUM1));
		table->Set(a_machine, "KEY_NUM2", gmVariable(PV_KEY_NUM2));
		table->Set(a_machine, "KEY_NUM3", gmVariable(PV_KEY_NUM3));
		table->Set(a_machine, "KEY_NUM4", gmVariable(PV_KEY_NUM4));
		table->Set(a_machine, "KEY_NUM5", gmVariable(PV_KEY_NUM5));
		table->Set(a_machine, "KEY_NUM6", gmVariable(PV_KEY_NUM6));
		table->Set(a_machine, "KEY_NUM7", gmVariable(PV_KEY_NUM7));
		table->Set(a_machine, "KEY_NUM8", gmVariable(PV_KEY_NUM8));
		table->Set(a_machine, "KEY_NUM9", gmVariable(PV_KEY_NUM9));
		table->Set(a_machine, "KEY_MULTIPLY", gmVariable(PV_KEY_MULTIPLY));
		table->Set(a_machine, "KEY_ADD", gmVariable(PV_KEY_ADD));
		table->Set(a_machine, "KEY_SUBTRACT", gmVariable(PV_KEY_SUBTRACT));
		table->Set(a_machine, "KEY_DECIMAL", gmVariable(PV_KEY_DECIMAL));
		table->Set(a_machine, "KEY_DIVIDE	", gmVariable(PV_KEY_DIVIDE));
		table->Set(a_machine, "KEY_F1", gmVariable(PV_KEY_F1));
		table->Set(a_machine, "KEY_F2", gmVariable(PV_KEY_F2));
		table->Set(a_machine, "KEY_F3", gmVariable(PV_KEY_F3));
		table->Set(a_machine, "KEY_F4", gmVariable(PV_KEY_F4));
		table->Set(a_machine, "KEY_F5", gmVariable(PV_KEY_F5));
		table->Set(a_machine, "KEY_F6", gmVariable(PV_KEY_F6));
		table->Set(a_machine, "KEY_F7", gmVariable(PV_KEY_F7));
		table->Set(a_machine, "KEY_F8", gmVariable(PV_KEY_F8));
		table->Set(a_machine, "KEY_F9", gmVariable(PV_KEY_F9));
		table->Set(a_machine, "KEY_F10", gmVariable(PV_KEY_F10));
		table->Set(a_machine, "KEY_F11", gmVariable(PV_KEY_F11));
		table->Set(a_machine, "KEY_F12", gmVariable(PV_KEY_F12));
		table->Set(a_machine, "KEY_F13", gmVariable(PV_KEY_F13));
		table->Set(a_machine, "KEY_F14", gmVariable(PV_KEY_F14));
		table->Set(a_machine, "KEY_F15", gmVariable(PV_KEY_F15));
		table->Set(a_machine, "KEY_F16", gmVariable(PV_KEY_F16));
		table->Set(a_machine, "KEY_F17", gmVariable(PV_KEY_F17));
		table->Set(a_machine, "KEY_F18", gmVariable(PV_KEY_F18));
		table->Set(a_machine, "KEY_F19", gmVariable(PV_KEY_F19));
		table->Set(a_machine, "KEY_F20", gmVariable(PV_KEY_F20));
		table->Set(a_machine, "KEY_F21", gmVariable(PV_KEY_F21));
		table->Set(a_machine, "KEY_F22", gmVariable(PV_KEY_F22));
		table->Set(a_machine, "KEY_F23", gmVariable(PV_KEY_F23));
		table->Set(a_machine, "KEY_F24", gmVariable(PV_KEY_F24));
		table->Set(a_machine, "KEY_NUMLOCK", gmVariable(PV_KEY_NUMLOCK));
		table->Set(a_machine, "KEY_SCROLLLOCK", gmVariable(PV_KEY_SCROLLLOCK));
		table->Set(a_machine, "KEY_LEFTSHIFT", gmVariable(PV_KEY_LEFTSHIFT));
		table->Set(a_machine, "KEY_RIGHTSHIFT", gmVariable(PV_KEY_RIGHTSHIFT));
		table->Set(a_machine, "KEY_LEFTCTRL", gmVariable(PV_KEY_LEFTCTRL));
		table->Set(a_machine, "KEY_RIGHTCTRL", gmVariable(PV_KEY_RIGHTCTRL));
	}

}