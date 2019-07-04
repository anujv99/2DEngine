#include "pch.h"
#include "virtualmachinelibs.h"

#include "gm/gmMathLib.h"
#include "gm/gmSystemLib.h"
#include "gm/gmArrayLib.h"
#include "gm/gmDebug.h"
#include "gm/gmMachineLib.h"

#include "gmbind/gmImmediateGfxLib.h"
#include "gmbind/gmimguilib.h"
#include "gmbind/gminputlib.h"

#include "gmbind/gmbindshader.h"
#include "gmbind/gmbindparticle.h"

namespace prev {

	void RegisterLibs(gmMachine * vm) {

		//System Libs
		gmBindMathLib(vm);
		gmBindDebugLib(vm);
		gmBindSystemLib(vm);
		gmBindArrayLib(vm);

		//Custom Libs
		gmBindImGuiLib(vm);
		gmBindImmediateGFXLib(vm);
		gmBindInputLib(vm);
		gmBindShaderManagerLib(vm);

		//Custom Types
		GM_BIND_INIT(VertexShader, vm);
		GM_BIND_INIT(PixelShader, vm);
		GM_BIND_INIT(ParticleSystem, vm);
	}

}
