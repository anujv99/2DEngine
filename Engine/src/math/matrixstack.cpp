#include "pch.h"
#include "matrixstack.h"

namespace prev {

	static const unsigned int MVP_MAX_STACK_ENTRIES = 64;

	MatrixStack::MatrixStack() : m_DirtyFlag(true) {
		m_Stack.push(Identity());
	}

	void MatrixStack::Push() {
		ASSERT(m_Stack.size() < MVP_MAX_STACK_ENTRIES);

		m_Stack.push(m_Stack.top());
	}

	void MatrixStack::Pop() {
		m_DirtyFlag = true;

		ASSERT(!m_Stack.empty());
		m_Stack.pop();
	}

	void MatrixStack::LoadIdentity() {
		Load(Identity());
	}

	void MatrixStack::Load(const Mat4 & mat) {
		m_DirtyFlag = true;
		Get() = mat;
	}

	void MatrixStack::Scale(const Vec2 & scale) {
		Scale(Vec3(scale.x, scale.y, 1.0f));
	}

	void MatrixStack::Scale(const Vec3 & scale) {
		m_DirtyFlag = true;
		Get() = prev::Scale(Get(), scale);
	}

	void MatrixStack::Translate(const Vec2 & position) {
		m_DirtyFlag = true;
		Get() = prev::Translate(Identity(), position);
	}

	void MatrixStack::Translate(const Vec3 & position) {
		m_DirtyFlag = true;
		Get() = prev::Translate(Get(), position);
	}

	void MatrixStack::Rotate(const float angle, const Vec3 & axis) {
		m_DirtyFlag = true;
		Get() = prev::Rotate(Get(), angle, axis);
	}

	void MatrixStack::Mul(const Mat4 & mat) {
		m_DirtyFlag = true;
		Get() *= mat;
	}

}