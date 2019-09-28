#pragma once

#include <fmod/fmod.hpp>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

#if defined(ENGINE_DEBUG)
	#define FMOD_CHECK_ERROR(F, M, ...) { if (!prev::FMODCheckError(F, M)) { __VA_ARGS__; } }
#else defined(ENGINE_REALEASE) || defined(ENGINE_DIST)
	#define FMOD_CHECK_ERROR(F, ...) F
#endif

namespace prev {

	inline bool FMODCheckError(FMOD_RESULT result, const std::string & message = "") {
		if (result != FMOD_OK) {
			LOG_ERROR("FMOD Error {} : {}", FMOD_ErrorString(result), message);
			return false;
		}
		return true;
	}

}
