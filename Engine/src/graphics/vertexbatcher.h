#pragma once

#include "vertexlayout.h"
#include "vertexshader.h"
#include "renderstate.h"

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace prev {

	class VertexBatcher {
	public:
		VertexBatcher(unsigned int reserveCount);
		~VertexBatcher();

		//Send Vertex
		void Vertex(Vec3 position);
		inline void Vertex(Vec2 position) { Vertex(Vec3(position)); }
		inline void Vertex(Vec3 position, Vec2 UV);
		inline void Vertex(float x, float y, float z = 0.0f) { Vertex(Vec3(x, y, z)); }

		void Color(Vec3 color, float alpha = 1.0f);
		inline void Color(Vec4 color) { m_CurrentColor = color; }
		inline void Color(float r, float g, float b, float a = 1.0f) { Color(Vec4(r, g, b, a)); }

		inline Vec4 GetColor() const { return m_CurrentColor; }

		//Begin End Shape
		virtual void PolygonBegin(PrimitiveTopology primitive) {}
		virtual void PolygonEnd(bool bindPixelShader = true) {}

		//2D
		void DrawLine(const Vec2 start, const Vec2 end);
		void DrawLineThick(const Vec2 start, const Vec2 end, const float thickenss);
		void DrawLineThickWire(const Vec2 start, const Vec2 end, const float thickenss);
		void DrawLineThickWithWire(const Vec2 start, const Vec2 end, const float thickenss);
		void DrawLineDashed(const Vec2 start, const Vec2 end, const float stepSize);
		void DrawSpline(const Vec2 v1, const Vec2 tan1, const Vec2 v2, const Vec2 tan2, 
			const float t1 = 0.0f, const float t2 = 1.0f, unsigned int numSteps = 64);
		void DrawTriangle(const Vec2 a, const Vec2 b, const Vec2 c);
		void DrawTriangleWire(const Vec2 a, const Vec2 b, const Vec2 c);
		void DrawRect(const Vec2 bottomLeft, const Vec2 dimen, float z = 0.0f);
		void DrawRectWire(const Vec2 bottomLeft, const Vec2 dimen);
		void DrawRectRounded(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTop(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTopWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottom(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottomWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius);
		void DrawQuad(const Vec2 p1, const Vec2 p2, const Vec2 p3, const Vec2 p4);
		void DrawQuadWire(const Vec2 p1, const Vec2 p2, const Vec2 p3, const Vec2 p4);

		//2D Circular
		void DrawCircle(const Vec2 center, const float radius, unsigned int segment = 32);
		void DrawCircleWire(const Vec2 center, const float radius, unsigned int segment = 32);
		void DrawEllipse(const Vec2 center, const Vec2 radius, unsigned int segment = 32);
		void DrawEllipseWire(const Vec2 center, const Vec2 radius, unsigned int segment = 32);
		void DrawDonut(const Vec2 center, const float radius, const float thickness, unsigned int segment = 32);
		void DrawDonutWire(const Vec2 center, const float radius, const float thickness, unsigned int segment = 32);
		void DrawArcDonut(const Vec2 center, const float radius, const float thickness, 
			const float startAngle, const float endAngle, unsigned int segment = 32);
		void DrawArcDonutWire(const Vec2 center, const float radius, const float thickness,
			const float startAngle, const float endAngle, unsigned int segment = 32);
		void DrawArcLine(const Vec2 center, const float radius, const float startAngle, const float endAngle, unsigned int segment = 32);

		StrongHandle<VertexLayout> CreateVertexLayout(StrongHandle<VertexShader> vShader) const;
	protected:
		struct VertexBatcherVert {
			Vec3 Position;
			Vec4 Color;
			Vec2 UV;
		};

		inline unsigned int GetNumBytesPerVertex() const { return sizeof(VertexBatcherVert); }
		inline std::vector<VertexBatcherVert> & GetVertexBatch() { return m_VertexBatch; }
	private:
		std::vector<Vec2> DrawRoundRectHelper(float startAngle, float radius, Vec2 startPos);
	private:
		Vec4 m_CurrentColor;
		std::vector<VertexBatcherVert> m_VertexBatch;
	};

}
