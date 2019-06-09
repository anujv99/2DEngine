#pragma once

#include "window.h"

namespace prev {

	class GraphicsContext : public Singleton<GraphicsContext> {
		friend class Singleton<GraphicsContext>;
	public:
		static std::vector<DisplayMode> GetDisplayModes();

		inline bool IsGraphicsContextReady() const { return m_IsContextCreated; }

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
	protected:
		GraphicsContext() {}
		virtual ~GraphicsContext() {}

		bool m_IsContextCreated = true;
	private:
		static GraphicsContext * CreateContext(const uintptr_t & windowRawPointer, const DisplayMode & displayMode);
	};

	template<>
	template<>
	inline void Singleton<GraphicsContext>::CreateInst(const uintptr_t & windowRawPointer, const DisplayMode & displayMode) {
		if (!s_Instance) s_Instance = GraphicsContext::CreateContext(windowRawPointer, displayMode);
	}

}
