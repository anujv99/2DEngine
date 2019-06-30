#include "pch.h"
#include "box2ddebugdraw.h"

#include "graphics/immediategfx.h"

namespace prev {

	void Box2DDebugDraw::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color) {
		ImmediateGFX::Ref().BeginDefaultShaders();
		ImmediateGFX::Ref().BeginDraw();
		ImmediateGFX::Ref().PolygonBegin(PV_PRIM_LINESTRIP);
		ImmediateGFX::Ref().Color(color.r, color.g, color.b, color.a);
		for (int i = 0; i < vertexCount; i++) {
			ImmediateGFX::Ref().Vertex(vertices[i]);
		}
		ImmediateGFX::Ref().Vertex(vertices[0]);
		ImmediateGFX::Ref().PolygonEnd();
	}

	void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color) {
		ImmediateGFX::Ref().BeginDefaultShaders();
		ImmediateGFX::Ref().BeginDraw();
		ImmediateGFX::Ref().PolygonBegin(PV_PRIM_TRIANGLELIST);
		ImmediateGFX::Ref().Color(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.6f);
		
		for (int32 i = 1; i < vertexCount - 1; ++i) {
			ImmediateGFX::Ref().Vertex(vertices[0]);
			ImmediateGFX::Ref().Vertex(vertices[i]);
			ImmediateGFX::Ref().Vertex(vertices[i + 1]);
		}

		ImmediateGFX::Ref().PolygonEnd();
	}

	void Box2DDebugDraw::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color) {
		ImmediateGFX::Ref().Color(color.r, color.g, color.b, color.a);
		ImmediateGFX::Ref().DrawCircleWire(center, radius);
	}

	void Box2DDebugDraw::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color) {
		ImmediateGFX::Ref().Color(color.r, color.g, color.b, color.a);
		ImmediateGFX::Ref().DrawCircle(center, radius);
	}

	void Box2DDebugDraw::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color) {
		ImmediateGFX::Ref().Color(color.r, color.g, color.b, color.a);
		ImmediateGFX::Ref().DrawLine(p1, p2);
	}

	void Box2DDebugDraw::DrawTransform(const b2Transform & xf) {
		const float32 k_axisScale = 0.4f;
		b2Color red(1.0f, 0.0f, 0.0f);
		b2Color green(0.0f, 1.0f, 0.0f);
		b2Vec2 p1 = xf.p, p2;

		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		ImmediateGFX::Ref().Color(red.r, red.g, red.b, red.a);
		ImmediateGFX::Ref().DrawLine(p1, p2);

		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		ImmediateGFX::Ref().Color(green.r, green.g, green.b, green.a);
		ImmediateGFX::Ref().DrawLine(p1, p2);
	}

	void Box2DDebugDraw::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color) {
		ImmediateGFX::Ref().Color(color.r, color.g, color.b, color.a);
		ImmediateGFX::Ref().DrawCircle(p, size);
	}

}