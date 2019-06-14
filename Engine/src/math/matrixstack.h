#pragma once

#include "mat4.h"

namespace prev {

	class MatrixStack {
	public:
		MatrixStack();

		void Push();
		void Pop();

		void LoadIdentity();
		void Load(const Mat4 & mat);

		void Scale(const Vec2 & scale);
		void Scale(const Vec3 & scale);
		void Translate(const Vec2 & position);
		void Translate(const Vec3 & position);
		void Rotate(const float angle, const Vec3 & axis);
		void Mul(const Mat4 & mat);

		const Mat4 Get() const { return m_Stack.top(); }
		Mat4 & Get() { return m_Stack.top(); }

		bool IsDirty() const { return m_DirtyFlag; }
		void SetDirtyOff() { m_DirtyFlag = false; }
	private:
		bool m_DirtyFlag;
		std::stack<Mat4> m_Stack;
	};

}
