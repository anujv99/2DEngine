#pragma once

namespace prev {

	inline uint32_t HashString(const std::string & str) {
		uint32_t hash = 0;
		for (unsigned int i = 0; i < str.size(); i++) {
			hash = 65599 * hash + (uint32_t)str[i];
		}
		return hash ^ (hash >> 16);
	}

	inline uint64_t HashUint32Pair(uint32_t val1, uint32_t val2) {
		return uint64_t(val1) | (uint64_t(val2) << 32);
	}

	inline uint64_t HashStringPair(const std::string & str1, const std::string & str2) {
		uint32_t hash1 = HashString(str1);
		uint32_t hash2 = HashString(str2);

		return HashUint32Pair(hash1, hash2);
	}

}
