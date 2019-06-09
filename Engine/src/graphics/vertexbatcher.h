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
		inline void Vertex(float x, float y, float z = 0.0f) { Vertex(Vec3(x, y, z)); }

		void Color(Vec3 color, float alpha = 1.0f);
		inline void Color(Vec4 color) { m_CurrentColor = color; }
		inline void Color(float r, float g, float b, float a = 1.0f) { Color(Vec4(r, g, b, a)); }

		inline Vec4 GetColor() const { return m_CurrentColor; }

		//Begin End Shape
		virtual void PolygonBegin(PrimitiveTopology primitive) {}
		virtual void PolygonEnd() {}

		//2D
		void DrawLine(const Vec2 start, const Vec2 end);

		StrongHandle<VertexLayout> CreateVertexLayout(StrongHandle<VertexShader> vShader) const;
	protected:
		struct VertexBatcherVert {
			Vec3 Position;
			Vec4 Color;
		};

		inline unsigned int GetNumBytesPerVertex() const { return sizeof(VertexBatcherVert); }
		inline std::vector<VertexBatcherVert> & GetVertexBatch() { return m_VertexBatch; }
	private:
		Vec4 m_CurrentColor;
		std::vector<VertexBatcherVert> m_VertexBatch;
	};

}
