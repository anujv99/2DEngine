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
#include "gmbind/gmcommonlib.h"

#include "gmbind/gmbindshader.h"
#include "gmbind/gmbindparticle.h"
#include "gmbind/gmbindgame.h"
#include "gmbind/gmbind.h"
#include "gmbind/gmbindgraphics.h"

namespace prev {

	void RegisterLibs(gmMachine * vm) {

		//System Libs
		gmBindMathLib(vm);
		gmBindDebugLib(vm);
		gmBindSystemLib(vm);
		gmBindArrayLib(vm);

		//Custom Libs
#ifdef IMGUI_ENABLED
		gmBindImGuiLib(vm);
#endif
		gmBindImmediateGFXLib(vm);
		gmBindInputLib(vm);
		gmBindShaderManagerLib(vm);
		gmBindCommonLib(vm);

		//Custom Types
		GM_BIND_INIT(VertexShader, vm);
		GM_BIND_INIT(PixelShader, vm);
		GM_BIND_INIT(ParticleSystem, vm);
		GM_BIND_INIT(Sprite, vm);
		GM_BIND_INIT(Label, vm);
		GM_BIND_INIT(Font, vm);
	}

}
