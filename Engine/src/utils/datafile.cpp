#include "pch.h"
#include "datafile.h"

#include "filehandler.h"

namespace prev {

	DataFile::DataFile() : m_Data("") {}

	DataFile::~DataFile() {
		Release();
	}

	void DataFile::Load(const std::string & fileName, bool isBinary /*= false*/) {
		if (m_Data.size() > 0) Release();

		m_Data = ReadFile(fileName, isBinary);
		if (m_Data.size() == 0)
			return;

		m_Ext = GetFileExt(fileName);
	}

	void DataFile::Release() {
		m_Data.clear();
	}

}