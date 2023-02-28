#include "pch.h"
#include "gmImmediategfxlib.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "gmbind.h"
#include "gmbindfuncgen.h"

#include "graphics/immediateGFX.h"

namespace prev {

	struct gmfImmediateGFXLib {

		GM_MEMFUNC_DECL(BeginDefaultShader) {
			GM_CHECK_NUM_PARAMS(0);
			ImmediateGFX::Ref().BeginDefaultShaders();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(EndDefaultShader) {
			GM_CHECK_NUM_PARAMS(0);
			ImmediateGFX::Ref().EndDefaultShaders();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(BeginDraw) {
			GM_CHECK_NUM_PARAMS(0);
			ImmediateGFX::Ref().BeginDraw();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(EndDraw) {
			GM_CHECK_NUM_PARAMS(0);
			ImmediateGFX::Ref().EndDraw();
			return GM_OK;
		}

		GM_MEMFUNC_DECL(PolygonBegin) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_INT_PARAM(drawPrimitive, 0);
			ImmediateGFX::Ref().PolygonBegin(static_cast<PrimitiveTopology>(drawPrimitive));
			return GM_OK;
		}

		GM_MEMFUNC_DECL(PolygonEnd) {
			GM_CHECK_NUM_PARAMS_RANGE(0, 1);
			GM_INT_PARAM(bindPS, 0, 1);
			ImmediateGFX::Ref().PolygonEnd(bindPS == 1);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(Color) {
			GM_CHECK_NUM_PARAMS(1);
			GM_CHECK_VEC4_PARAM(color, 0);

			ImmediateGFX::Ref().Color(color);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(Vertex) {
			GM_CHECK_NUM_PARAMS_RANGE(1, 2);
			GM_CHECK_VEC2_PARAM(pos, 0);
			GM_VEC2_PARAM(uv, 1);

			ImmediateGFX::Ref().Vertex(Vec3(pos), uv);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(DrawCircle) {
			GM_CHECK_NUM_PARAMS_RANGE(2, 3);
			GM_CHECK_VEC2_PARAM(center, 0);
			GM_CHECK_FLOAT_OR_INT_PARAM(radius, 1);
			GM_INT_PARAM(numSegs, 2, 32);

			ImmediateGFX::Ref().DrawCircle(center, radius, numSegs);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(DrawCircleWire) {
			GM_CHECK_NUM_PARAMS_RANGE(2, 3);
			GM_CHECK_VEC2_PARAM(center, 0);
			GM_CHECK_FLOAT_OR_INT_PARAM(radius, 1);
			GM_INT_PARAM(numSegs, 2, 32);

			ImmediateGFX::Ref().DrawCircleWire(center, radius, numSegs);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(DrawEllipseWire) {
			GM_CHECK_NUM_PARAMS_RANGE(2, 3);
			GM_CHECK_VEC2_PARAM(center, 0);
			GM_CHECK_VEC2_PARAM(radius, 1);
			GM_INT_PARAM(numSegs, 2, 32);

			ImmediateGFX::Ref().DrawEllipseWire(center, radius, numSegs);
			return GM_OK;
		}

		GM_MEMFUNC_DECL(DrawQuad) {
			GM_CHECK_NUM_PARAMS(4);
			GM_CHECK_VEC2_PARAM(p1, 0);
			GM_CHECK_VEC2_PARAM(p2, 1);
			GM_CHECK_VEC2_PARAM(p3, 2);
			GM_CHECK_VEC2_PARAM(p4, 3);

			ImmediateGFX::Ref().DrawQuad(p1, p2, p3, p4);
			return GM_OK;
		}

	};

	static gmFunctionEntry s_ImmediateGFXLib[] = {
		GM_LIBFUNC_ENTRY(BeginDefaultShader, ImmediateGFX)
		GM_LIBFUNC_ENTRY(EndDefaultShader, ImmediateGFX)
		GM_LIBFUNC_ENTRY(BeginDraw, ImmediateGFX)
		GM_LIBFUNC_ENTRY(EndDraw, ImmediateGFX)
		GM_LIBFUNC_ENTRY(PolygonBegin, ImmediateGFX)
		GM_LIBFUNC_ENTRY(PolygonEnd, ImmediateGFX)
		GM_LIBFUNC_ENTRY(Color, ImmediateGFX)
		GM_LIBFUNC_ENTRY(Vertex, ImmediateGFX)
		GM_LIBFUNC_ENTRY(DrawCircle, ImmediateGFX)
		GM_LIBFUNC_ENTRY(DrawCircleWire, ImmediateGFX)
		GM_LIBFUNC_ENTRY(DrawEllipseWire, ImmediateGFX)
		GM_LIBFUNC_ENTRY(DrawQuad, ImmediateGFX)
	};

	void gmBindImmediateGFXLib(gmMachine * a_machine) {
		a_machine->RegisterLibrary(s_ImmediateGFXLib, GM_LIBFUNC_SIZE(s_ImmediateGFXLib), "Imm");
	}

}