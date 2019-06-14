#include "pch.h"
#include "vertexbatcher.h"

namespace prev {

	VertexBatcher::VertexBatcher(unsigned int reserveCount) {
		m_VertexBatch.reserve(reserveCount);

		m_CurrentColor = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	VertexBatcher::~VertexBatcher() {
	}

	void VertexBatcher::Vertex(Vec3 position) {
		ASSERT(m_VertexBatch.size() < m_VertexBatch.capacity());

		VertexBatcherVert vert;
		vert.Position = position;
		vert.Color = m_CurrentColor;
		vert.UV = Vec2(0.0f);

		m_VertexBatch.push_back(vert);
	}

	void VertexBatcher::Vertex(Vec3 position, Vec2 UV) {
		ASSERT(m_VertexBatch.size() < m_VertexBatch.capacity());

		VertexBatcherVert vert;
		vert.Position = position;
		vert.Color = m_CurrentColor;
		vert.UV = UV;

		m_VertexBatch.push_back(vert);
	}

	void VertexBatcher::Color(Vec3 color, float alpha /*= 1.0f*/) {
		m_CurrentColor.r = color.r;
		m_CurrentColor.g = color.g;
		m_CurrentColor.b = color.b;
		m_CurrentColor.a = alpha;
	}

	void VertexBatcher::DrawLine(const Vec2 start, const Vec2 end) {
		PolygonBegin(PV_PRIM_LINELIST);
		RenderState::Ref().SetPrimitiveTopology(PV_PRIM_LINELIST);
		Vertex(start);
		Vertex(end);
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThick(const Vec2 start, const Vec2 end, const float thickenss) {
		Vec2 dir = end - start;
		float len = Length(dir);
		if (len < 0.01f) return;

		Vec2 dir_normalized = dir / len;
		Vec2 dir_perp = Perpendicular(dir_normalized);
		Vec2 dir_opposite_top = dir_perp * thickenss * 0.5f;
		Vec2 dir_opposite_bot = -dir_perp * thickenss * 0.5f;

		Vec2 pos0 = start + dir_opposite_top;
		Vec2 pos1 = start + dir_opposite_bot;
		Vec2 pos2 = end + dir_opposite_bot;
		Vec2 pos3 = end + dir_opposite_top;

		PolygonBegin(PV_PRIM_TRIANGLELIST);
		Vertex(pos0);
		Vertex(pos1);
		Vertex(pos2);
		Vertex(pos0);
		Vertex(pos2);
		Vertex(pos3);
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThickWire(const Vec2 start, const Vec2 end, const float thickenss) {
		Vec2 dir = end - start;
		float len = Length(dir);
		if (len < 0.01f) return;

		Vec2 dir_normalized = dir / len;
		Vec2 dir_perp = Perpendicular(dir_normalized);
		Vec2 dir_opposite_top = dir_perp * thickenss * 0.5f;
		Vec2 dir_opposite_bot = -dir_perp * thickenss * 0.5f;

		Vec2 pos0 = start + dir_opposite_top;
		Vec2 pos1 = start + dir_opposite_bot;
		Vec2 pos2 = end + dir_opposite_bot;
		Vec2 pos3 = end + dir_opposite_top;

		PolygonBegin(PV_PRIM_LINESTRIP);
		Vertex(pos0);
		Vertex(pos1);
		Vertex(pos2);
		Vertex(pos3);
		Vertex(pos0);
		PolygonEnd();
	}

	void VertexBatcher::DrawLineThickWithWire(const Vec2 start, const Vec2 end, const float thickenss) {
		Vec2 dir = end - start;
		float len = Length(dir);
		if (len < 0.01f) return;

		Vec2 dir_normalized = dir / len;
		Vec2 dir_perp = Perpendicular(dir_normalized);
		Vec2 dir_opposite_top = dir_perp * thickenss * 0.5f;
		Vec2 dir_opposite_bot = -dir_perp * thickenss * 0.5f;

		Vec2 pos0 = start + dir_opposite_top;
		Vec2 pos1 = start + dir_opposite_bot;
		Vec2 pos2 = end + dir_opposite_bot;
		Vec2 pos3 = end + dir_opposite_top;

		PolygonBegin(PV_PRIM_TRIANGLELIST);
		Vertex(pos0);
		Vertex(pos1);
		Vertex(pos2);
		Vertex(pos0);
		Vertex(pos2);
		Vertex(pos3);
		PolygonEnd();

		PolygonBegin(PV_PRIM_LINESTRIP);
		Vertex(pos0);
		Vertex(pos1);
		Vertex(pos2);
		Vertex(pos3);
		Vertex(pos0);
		PolygonEnd();
	}

	void VertexBatcher::DrawLineDashed(const Vec2 start, const Vec2 end, const float stepSize) {
		const Vec2 diff = end - start;
		const float len = Length(diff);

		if (len < 0.01f || stepSize < 0.01f) return;

		const Vec2 dir = diff / len;
		const Vec2 dir_step = dir * stepSize;
		const int num_steps = int(len / stepSize);

		PolygonBegin(PV_PRIM_LINELIST);

		for (int i = 0; i < num_steps; i += 2) {
			float index = float(i);

			Vec2 pos0 = start + dir_step * index;
			Vec2 pos1 = start + dir_step * (index + 1.0f);

			Vertex(pos0);
			Vertex(pos1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawSpline(const Vec2 v1, const Vec2 tan1, const Vec2 v2, const Vec2 tan2, 
		const float t1 /*= 0.0f*/, const float t2 /*= 1.0f*/, unsigned int numSteps /*= 64*/) {

		const float deltaT = (t2 - t1) / (numSteps - 1);

		PolygonBegin(PV_PRIM_LINESTRIP);
		for (unsigned int i = 0; i < numSteps; ++i) {
			const float t = t1 + deltaT * i;
			Vec2 pos = Hermite(v1, tan1, v2, tan2, t);

			Vertex(pos.x, pos.y);
		}
		PolygonEnd();

	}

	void VertexBatcher::DrawTriangle(const Vec2 a, const Vec2 b, const Vec2 c) {
		PolygonBegin(PV_PRIM_TRIANGLELIST);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		PolygonEnd();
	}

	void VertexBatcher::DrawTriangleWire(const Vec2 a, const Vec2 b, const Vec2 c) {
		PolygonBegin(PV_PRIM_LINESTRIP);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		Vertex(a);
		PolygonEnd();
	}

	void VertexBatcher::DrawRect(const Vec2 bottomLeft, const Vec2 dimen, float z /*= 0.0f*/) {
		Vec3 pos[4] = {
			Vec3(bottomLeft.x, bottomLeft.y, z),
			Vec3(bottomLeft.x + dimen.x, bottomLeft.y, z),
			Vec3(bottomLeft.x + dimen.x, bottomLeft.y + dimen.y, z),
			Vec3(bottomLeft.x, bottomLeft.y + dimen.y, z)
		};

		Vec2 uv[4] = {
			Vec2(0.0f,1.0f),
			Vec2(1.0f,1.0f),
			Vec2(1.0f,0.0f),
			Vec2(0.0f,0.0f),
		};

		PolygonBegin(PV_PRIM_TRIANGLELIST);
		Vertex(pos[0], uv[0]);
		Vertex(pos[1], uv[1]);
		Vertex(pos[2], uv[2]);
		Vertex(pos[0], uv[0]);
		Vertex(pos[2], uv[2]);
		Vertex(pos[3], uv[3]);
		PolygonEnd();

	}

	void VertexBatcher::DrawRectWire(const Vec2 bottomLeft, const Vec2 dimen) {
		PolygonBegin(PV_PRIM_LINESTRIP);
		Vertex(bottomLeft.x + dimen.x, bottomLeft.y);
		Vertex(bottomLeft.x, bottomLeft.y);
		Vertex(bottomLeft.x, bottomLeft.y + dimen.y);
		Vertex(bottomLeft.x + dimen.x, bottomLeft.y + dimen.y);
		Vertex(bottomLeft.x + dimen.x, bottomLeft.y);
		PolygonEnd();
	}

	void VertexBatcher::DrawRectRounded(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_TRIANGLELIST);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 180;
		std::vector<Vec2> positions;

		Vec2 startPositions[4] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 4; j++) {
			positions = DrawRoundRectHelper(angle , cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			Vertex(centerPos);
			Vertex(positions[positions.size() - 1]);

			angle += 90.0f;
		}

		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawRectRoundedWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_LINELIST);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 180;
		std::vector<Vec2> positions;

		Vec2 startPositions[4] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 4; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			Vertex(positions[positions.size() - 1]);

			angle += 90.0f;
		}

		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawRectRoundedTop(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_TRIANGLELIST);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 0;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(centerPos);
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft);
		Vertex(centerPos);

		Vertex(bottomLeft);
		Vertex(centerPos);
		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));

		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));
		Vertex(centerPos);
		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawRectRoundedTopWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_LINELIST);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 0;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft);

		Vertex(bottomLeft);
		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));

		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));
		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawRectRoundedBottom(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_TRIANGLELIST);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 180.0f;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(centerPos);
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft + dimen);
		Vertex(centerPos);

		Vertex(bottomLeft + dimen);
		Vertex(centerPos);
		Vertex(bottomLeft + Vec2(0.0f, dimen.y));

		Vertex(bottomLeft + Vec2(0.0f, dimen.y));
		Vertex(centerPos);
		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawRectRoundedBottomWire(const Vec2 bottomLeft, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(PV_PRIM_LINESTRIP);
		const Vec2 centerPos = bottomLeft + dimen * 0.5f;
		Vec2 firstPos;
		float angle = 180.0f;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft + dimen);

		Vertex(bottomLeft + dimen);
		Vertex(bottomLeft + Vec2(0.0f, dimen.y));

		Vertex(bottomLeft + Vec2(0.0f, dimen.y));
		Vertex(firstPos);

		PolygonEnd();
	}

	void VertexBatcher::DrawQuad(const Vec2 p1, const Vec2 p2, const Vec2 p3, const Vec2 p4) {
		PolygonBegin(PV_PRIM_TRIANGLELIST);
		Vertex(p1);
		Vertex(p2);
		Vertex(p3);

		Vertex(p1);
		Vertex(p3);
		Vertex(p4);
		PolygonEnd();
	}

	void VertexBatcher::DrawQuadWire(const Vec2 p1, const Vec2 p2, const Vec2 p3, const Vec2 p4) {
		PolygonBegin(PV_PRIM_LINESTRIP);
		Vertex(p1);
		Vertex(p2);
		Vertex(p3);
		Vertex(p4);
		Vertex(p1);
		PolygonEnd();
	}

	void VertexBatcher::DrawCircle(const Vec2 center, const float radius, unsigned int segment /*= 32*/) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_TRIANGLELIST);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + UnitCircle(angle_1) * radius;

			Vertex(center);
			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawCircleWire(const Vec2 center, const float radius, unsigned int segment /*= 32*/) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_LINESTRIP);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + UnitCircle(angle_1) * radius;

			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawEllipse(const Vec2 center, const Vec2 radius, unsigned int segment /*= 32*/) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_TRIANGLELIST);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + UnitCircle(angle_1) * radius;

			Vertex(center);
			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawEllipseWire(const Vec2 center, const Vec2 radius, unsigned int segment /*= 32*/) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_LINESTRIP);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + UnitCircle(angle_1) * radius;

			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawDonut(const Vec2 center, const float radius, const float thickness, unsigned int segment /*= 32*/) {
		float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_TRIANGLESTRIP);

		for (unsigned int i = 0; i <= segment; ++i) {
			float currAngle = delta_angle * i;

			Vec2 dir = Vec2(Cos(currAngle), Sin(currAngle));
			Vec2 pos0 = center + (radius - 0.5f * thickness) * dir;
			Vec2 pos1 = center + (radius + 0.5f * thickness) * dir;

			Vertex(pos0);
			Vertex(pos1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawDonutWire(const Vec2 center, const float radius, const float thickness, unsigned int segment /*= 32*/) {
		float delta_angle = 360.0f / segment;

		PolygonBegin(PV_PRIM_LINELIST);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1.0f);

			Vec2 dir_0 = Vec2(Cos(angle_0), Sin(angle_0));
			Vec2 dir_1 = Vec2(Cos(angle_1), Sin(angle_1));

			Vec2 pos_0_0 = center + (radius - 0.5f * thickness) * dir_0;
			Vec2 pos_0_1 = center + (radius - 0.5f * thickness) * dir_1;

			Vec2 pos_1_0 = center + (radius + 0.5f * thickness) * dir_0;
			Vec2 pos_1_1 = center + (radius + 0.5f * thickness) * dir_1;

			Vertex(pos_0_0);
			Vertex(pos_0_1);
			Vertex(pos_1_0);
			Vertex(pos_1_1);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawArcDonut(const Vec2 center, const float radius, const float thickness, 
		const float startAngle, const float endAngle, int segment /*= 32*/) {

		float delta_angle = (endAngle - startAngle) / (segment);

		PolygonBegin(PV_PRIM_TRIANGLESTRIP);

		for (unsigned int i = 0; i <= segment; ++i) {
			float currAngle = startAngle + delta_angle * i;

			Vec2 dir = Vec2(Cos(currAngle), Sin(currAngle));
			Vec2 pos0 = center + (radius - 0.5f * thickness) * dir;
			Vec2 pos1 = center + (radius + 0.5f * thickness) * dir;

			Vertex(pos0.x, pos0.y);
			Vertex(pos1.x, pos1.y);
		}

		PolygonEnd();
	}

	void VertexBatcher::DrawArcDonutWire(const Vec2 center, const float radius, const float thickness, 
		const float startAngle, const float endAngle, int segment /*= 32*/) {

		float delta_angle = (endAngle - startAngle) / (segment);

		PolygonBegin(PV_PRIM_LINELIST);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = startAngle + delta_angle * i;
			float angle_1 = startAngle + delta_angle * (i + 1.0f);

			Vec2 dir_0 = Vec2(Cos(angle_0), Sin(angle_0));
			Vec2 dir_1 = Vec2(Cos(angle_1), Sin(angle_1));

			Vec2 pos_0_0 = center + (radius - 0.5f * thickness) * dir_0;
			Vec2 pos_0_1 = center + (radius - 0.5f * thickness) * dir_1;
			Vec2 pos_1_0 = center + (radius + 0.5f * thickness) * dir_0;
			Vec2 pos_1_1 = center + (radius + 0.5f * thickness) * dir_1;

			Vertex(pos_0_0);
			Vertex(pos_0_1);
			Vertex(pos_1_0);
			Vertex(pos_1_1);
		}

		PolygonEnd();

	}

	void VertexBatcher::DrawArcLine(const Vec2 center, const float radius, const float startAngle, const float endAngle, int segment /*= 32*/) {
		float delta_angle = (endAngle - startAngle) / (segment - 1);

		PolygonBegin(PV_PRIM_LINELIST);

		for (unsigned int i = 0; i < segment - 1; ++i) {
			float currAngle = startAngle + delta_angle * i;
			float nextAngle = startAngle + delta_angle * (i + 1);

			float x0 = center.x + radius * Cos(currAngle);
			float y0 = center.y + radius * Sin(currAngle);
			float x1 = center.x + radius * Cos(nextAngle);
			float y1 = center.y + radius * Sin(nextAngle);

			Vertex(x0, y0);
			Vertex(x1, y1);
		}

		PolygonEnd();
	}

	prev::StrongHandle<prev::VertexLayout> VertexBatcher::CreateVertexLayout(StrongHandle<VertexShader> vShader) const {
		StrongHandle<VertexLayout> vertexLayout = VertexLayout::CreateVertexLayout();

		vertexLayout->BeginEntries();
		vertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 3, offsetof(VertexBatcherVert, Position), "POSITION", false);
		vertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 4, offsetof(VertexBatcherVert, Color), "COLOR", false);
		vertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(VertexBatcherVert, UV), "TEXCOORDS", false);
		vertexLayout->EndEntries(vShader);

		return vertexLayout;
	}

	std::vector<Vec2> VertexBatcher::DrawRoundRectHelper(float startAngle, float radius, Vec2 startPos) {
		const int numRoundedVerts = 4;
		const float roundedCornerAngle = 90.0f;
		const float roundedCircledelta_angle = roundedCornerAngle / (numRoundedVerts - 1);

		std::vector<Vec2> positions;

		for (unsigned int i = 0; i < numRoundedVerts; ++i) {
			float angleRad = startAngle + i * roundedCircledelta_angle;
			Vec2 unitCircle = UnitCircle(angleRad);
			Vec2 pos = startPos + unitCircle * radius;
			positions.push_back(pos);
		}

		return positions;
	}

}