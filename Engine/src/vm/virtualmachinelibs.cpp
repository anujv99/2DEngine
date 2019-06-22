#include "pch.h"
#include "virtualmachinelibs.h"

#include "gm/gmMathLib.h"
#include "imgui/imguigm.h"

namespace prev {

	void RegisterLibs(gmMachine * vm) {

		//InitLibs
		gmBindMathLib(vm);

		//CustomLibs
		gmBindImGuiLib(vm);

	}

}
