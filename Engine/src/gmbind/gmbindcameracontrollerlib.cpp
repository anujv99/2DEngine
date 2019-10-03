#include "pch.h"
#include "gmbindcameracontrollerlib.h"

#include "gmbind/gmbind.h"
#include "gmbind/gmbindfuncgen.h"

#include "game/cameracontroller.h"

namespace prev {

	struct gmfCameraControllerLib {

		GM_MEMFUNC_DECL(GetXCoords) {
			GM_CHECK_NUM_PARAMS(0);
			a_thread->PushVec2(CameraController::Ref().GetScreenCoordsX());
			return GM_OK;
		}

		GM_MEMFUNC_DECL(GetYCoords) {
			GM_CHECK_NUM_PARAMS(0);
			a_thread->PushVec2(CameraController::Ref().GetScreenCoordsY());
			return GM_OK;
		}

	};

	static gmFunctionEntry s_CameraControllerLib[] = {
		GM_LIBFUNC_ENTRY(GetXCoords, CameraController)
		GM_LIBFUNC_ENTRY(GetYCoords, CameraController)
	};

	void gmBindCameraControllerLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_CameraControllerLib, GM_LIBFUNC_SIZE(s_CameraControllerLib), "CameraController");
	}

}