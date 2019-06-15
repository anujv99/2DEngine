#pragma once

namespace prev {

	std::string ReadFile(const std::string & fileName, bool isBinary = false);
	bool FileExists(const std::string & fileName);

	inline std::string GetFileExt(const std::string & fileName) {
		return fileName.substr(fileName.find_last_of(".") + 1);
	}
}
