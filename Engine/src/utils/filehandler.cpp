#include "pch.h"
#include "filehandler.h"

#include <ios>

namespace prev {

	std::string ReadFile(const std::string & fileName, bool isBinary /*= false*/) {
		std::ifstream inFile;
		if (isBinary)
			inFile.open(fileName, std::ios::binary | std::ios::app);
		else
			inFile.open(fileName);

		if (!inFile) {
			ERROR_TRACE(ERR_FILE_NOT_FOUND, "Script file " + fileName + " not found");
			return "";
		}

		std::stringstream sstr;

		sstr << inFile.rdbuf();

		inFile.close();

		return sstr.str();
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