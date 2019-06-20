#pragma once

#include "gm/gmVariable.h"

namespace prev {

	class VirtualMachine : public Singleton<VirtualMachine> {
		friend Singleton<VirtualMachine>;
	private:
		VirtualMachine();
		~VirtualMachine();
	public:
		void Update();
		void Render();
		void RunMain();
	private:
		void HandleErrors();

		void InitDrawManager();
		void InitGlobals();
	private:
		int m_NumThreads;
		int m_ThreadID;

		gmMachine * m_VM;

		gmFunctionObject * m_DrawFunction;
		gmFunctionObject * m_ClearFunction;
		gmVariable m_DrawManager;

		float m_UpdateMs;
		float m_DrawMs;
		unsigned int m_LastCallTime;
	};

}
