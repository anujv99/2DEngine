#pragma once

#include "matrixstack.h"

namespace prev {

	class MVP : public Singleton<MVP> {
		friend class Singleton<MVP>;
	private:
		MVP();
		~MVP();
	public:
		MatrixStack & Model() { return m_MatrixStack[0]; }
		MatrixStack & View() { return m_MatrixStack[1]; }
		MatrixStack & Projection() { return m_MatrixStack[2]; }

		const MatrixStack & Model() const { return m_MatrixStack[0]; }
		const MatrixStack & View() const { return m_MatrixStack[1]; }
		const MatrixStack & Projection() const { return m_MatrixStack[2]; }

		MatrixStack & GetStack(int index) { ASSERT(index < 3); return m_MatrixStack[index]; }
		const MatrixStack & GetStack(int index) const { ASSERT(index < 3); return m_MatrixStack[index]; }

		Mat4 Get();

		bool IsDirty();
	public:
		static const float s_Near;
		static const float s_Far;
		static const float s_YScale;
		static float s_XScale;
	private:
		void EventFunction(Event & e);
		bool WindowResize(WindowResizeEvent & e);
	private:
		MatrixStack m_MatrixStack[3]; //Model, View, Projection
		Mat4 m_MVPCache;

		//Resolution Handling
		Vec2 m_WindowSize;
	};

}
