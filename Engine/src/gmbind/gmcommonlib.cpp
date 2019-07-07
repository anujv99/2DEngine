#include "pch.h"
#include "gmcommonlib.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "common/timer.h"

#include "gmbind.h"
#include "gmbindfuncgen.h"

namespace prev {

	struct gmfTimerLib {

		GM_MEMFUNC_DECL(GetTime) {
			GM_CHECK_NUM_PARAMS(0);
			float time = Timer::GetTime();
			a_thread->PushFloat(time);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetTimeMs) {
			GM_CHECK_NUM_PARAMS(0);
			float time = Timer::GetTimeMs();
			a_thread->PushFloat(time);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetDeltaTime) {
			GM_CHECK_NUM_PARAMS(0);
			float time = Timer::GetDeltaTime();
			a_thread->PushFloat(time);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetDeltaTimeMs) {
			GM_CHECK_NUM_PARAMS(0);
			float time = Timer::GetDeltaTime() * 1000.0f;
			a_thread->PushFloat(time);
			return GM_OK;
		}

	};

	static gmFunctionEntry s_gmTimerLib[] = {
		GM_LIBFUNC_ENTRY(GetTime, Timer)
		GM_LIBFUNC_ENTRY(GetTimeMs, Timer)
		GM_LIBFUNC_ENTRY(GetDeltaTime, Timer)
		GM_LIBFUNC_ENTRY(GetDeltaTimeMs, Timer)
	};

	void gmBindCommonLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_gmTimerLib, GM_LIBFUNC_SIZE(s_gmTimerLib), "Timer");
	}

}