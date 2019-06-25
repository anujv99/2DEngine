#include "pch.h"
#include "virtualmachinelibs.h"

#include "gm/gmMathLib.h"
#include "gm/gmDebug.h"
#include "gmbind/gmImmediateGfxLib.h"

#include "gmbind/gmimguilib.h"

namespace prev {

	void RegisterLibs(gmMachine * vm) {

		//System Libs
		gmBindMathLib(vm);
		gmBindDebugLib(vm);

		//Custom Libs
		gmBindImGuiLib(vm);
		gmBindImmediateGFXLib(vm);

		//CustomLibs
	}

}
