#include "pch.h"
#include "gmcommonlib.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "common/timer.h"

#include "gmbind.h"
#include "gmbindfuncgen.h"

#include <random>

namespace prev {

	static std::mt19937_64 s_Rng;

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

	struct gmfRandomLib {

		GM_MEMFUNC_DECL(Srand) {
			GM_CHECK_NUM_PARAMS(1);
			GM_FLOAT_OR_INT_PARAM(seed, 0, 0.0f);
			unsigned int seedUint = (unsigned int)seed;
			std::srand(seedUint);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(TimeSeed) {
			GM_CHECK_NUM_PARAMS(0);
			uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			std::seed_seq ss{(uint32_t)timeSeed & 0xffffffff, (uint32_t)timeSeed >> 32};
			s_Rng.seed(ss);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(Rand) {
			GM_CHECK_NUM_PARAMS(2);
			GM_FLOAT_OR_INT_PARAM(min, 0, 0.0f);
			GM_FLOAT_OR_INT_PARAM(max, 1, 1.0f);

			if (min > max) {
				auto temp = min;
				min = max;
				max = temp;
			}

			std::uniform_real_distribution<double> unif(min, max);
			float random = unif(s_Rng);

			a_thread->PushFloat((float)random);
			return GM_OK;
		}

	};

	static gmFunctionEntry s_gmTimerLib[] = {
		GM_LIBFUNC_ENTRY(GetTime, Timer)
		GM_LIBFUNC_ENTRY(GetTimeMs, Timer)
		GM_LIBFUNC_ENTRY(GetDeltaTime, Timer)
		GM_LIBFUNC_ENTRY(GetDeltaTimeMs, Timer)
	};

	static gmFunctionEntry s_gmRandomLib[] = {
		GM_LIBFUNC_ENTRY(Srand, Random)
		GM_LIBFUNC_ENTRY(TimeSeed, Random)
		GM_LIBFUNC_ENTRY(Rand, Random)
	};

	void gmBindCommonLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_gmTimerLib, GM_LIBFUNC_SIZE(s_gmTimerLib), "Timer");
		a_machine->RegisterLibrary(s_gmRandomLib, GM_LIBFUNC_SIZE(s_gmRandomLib), "Random");
	}

}