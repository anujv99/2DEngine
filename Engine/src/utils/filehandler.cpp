#include "pch.h"
#include "filehandler.h"

#include <ios>

namespace prev {

	const char * ReadFile(const std::string & fileName, unsigned int * fileSize /*= nullptr*/, bool isBinary /*= false*/) {
		std::ifstream inFile;
		if (isBinary)
			inFile.open(fileName, std::ios::binary | std::ios::app);
		else
			inFile.open(fileName);

		if (!inFile) {
			LOG_ERROR("Unable to open file : " + fileName);
			return nullptr;
		}

		inFile.seekg(0, std::ios::end);
		size_t size = (size_t)inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		char * data = new char[size + 1];
		inFile.read(data, size);
		inFile.close();

		data[size] = '\0';

		if (fileSize) * fileSize = size;

		return data;
	}

	bool FileExists(const std::string & fileName) {
		if (fileName.size() == 0) {
			LOG_ERROR("Invalid file name");
			return false;
		}
		std::ifstream inFile(fileName);
		return inFile ? true : false;
	}

}