#include "pch.h"
#include "virtualmachine.h"

namespace prev {

	VirtualMachine::VirtualMachine() : 
		m_NumThreads(0), m_ThreadID(0), 
		m_DrawMs(0.0f), m_UpdateMs(0.0f),
		m_VM(nullptr),
		m_DrawFunction(nullptr), m_ClearFunction(nullptr), m_DrawManager(GM_NULL) {

	}

	VirtualMachine::~VirtualMachine() {

	}

	void VirtualMachine::Update() {

	}

	void VirtualMachine::Render() {

	}

	void VirtualMachine::RunMain() {

	}

	void VirtualMachine::HandleErrors() {

	}

	void VirtualMachine::InitDrawManager() {

	}

	void VirtualMachine::InitGlobals() {

	}

}