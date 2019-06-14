#pragma once

namespace prev {

	const char * ReadFile(const std::string & fileName, unsigned int * fileSize = nullptr, bool isBinary = false);
	bool FileExists(const std::string & fileName);

	inline std::string GetFileExt(const std::string & fileName) {
		return fileName.substr(fileName.find_last_of(".") + 1);
	}
}
