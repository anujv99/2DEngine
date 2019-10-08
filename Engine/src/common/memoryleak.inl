#pragma once

#include <crtdbg.h>

struct MemoryLeakDetector {

	MemoryLeakDetector() {
		_CrtMemCheckpoint(&m_StartMemset);
	}

	~MemoryLeakDetector() {
		_CrtMemCheckpoint(&m_EndMemset);
		_CrtMemDifference(&m_DiffMemset, &m_StartMemset, &m_EndMemset);
		_CrtDumpMemoryLeaks();
	}

	// Memory block types: Free, Normal, CRT, Ignore, Client
	_CrtMemState m_StartMemset = {};
	_CrtMemState m_EndMemset = {};
	_CrtMemState m_DiffMemset = {};

};

static MemoryLeakDetector gs_MemoryLeakDetector;
