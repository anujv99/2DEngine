#include "pch.h"
#include "datafile.h"

#include "filehandler.h"

namespace prev {

	DataFile::DataFile() : m_Data(nullptr), m_NumBytes(0u) {}

	DataFile::~DataFile() {
		Release();
	}

	void DataFile::Load(const std::string & fileName, bool isBinary /*= false*/) {
		if (m_Data) Release();

		m_Data = (char *)ReadFile(fileName, &m_NumBytes, isBinary);
		if (m_Data == nullptr)
			return;

		m_Ext = GetFileExt(fileName);
	}

	void DataFile::Release() {
		delete[] m_Data;
		m_Data = nullptr;
		m_NumBytes = 0u;
	}

}