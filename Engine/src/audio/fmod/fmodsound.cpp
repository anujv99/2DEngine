#include "pch.h"
#include "fmodsound.h"

#if defined(ENGINE_DEBUG)
	#define FMOD_CHECK_SOUND(...) if (!m_IsLoaded) { LOG_ERROR("FMOD sound not loaded. Unable to call function : {}", __FUNCTION__); __VA_ARGS__; }
#else defined(ENGINE_REALEASE) || defined(ENGINE_DIST)
	#define FMOD_CHECK_SOUND(...)
#endif

namespace prev {

	FMODSound::FMODSound(const std::string & fileName, FMOD::Studio::System * system) :
		m_Sound(nullptr), m_Channel(nullptr), 
		m_FileName(""), m_IsLoaded(false) {

		m_IsLoaded = Initialize(fileName, system);

		if (!m_IsLoaded) {
			LOG_ERROR("Unable to play sound file : {}", fileName);
		}
	}

	FMODSound::~FMODSound() {
		m_Channel->stop();
	}

	bool FMODSound::Initialize(const std::string & fileName, FMOD::Studio::System * system) {
		if (system == nullptr) {
			LOG_ERROR("Inavlid FMOD system");
			return false;
		}

		FMOD::System * coreSystem = nullptr;

		FMOD_CHECK_ERROR(system->getCoreSystem(&coreSystem), "Unable to get FMOD core system", return false);
		FMOD_CHECK_ERROR(coreSystem->createSound(fileName.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &m_Sound), "Unable to create sound", return false);
		FMOD_CHECK_ERROR(coreSystem->playSound(m_Sound, nullptr, true, &m_Channel), "Unable to play sound", return false);

		m_FileName = fileName;

		return true;
	}

	void FMODSound::Play() {
		FMOD_CHECK_SOUND(return);
		bool isPaused = false;
		FMOD_CHECK_ERROR(m_Channel->getPaused(&isPaused), "Unable to check sound status", return);

		if (!isPaused)
			return;

		FMOD_CHECK_ERROR(m_Channel->setPaused(false), "Unable to set sound status", return);
	}

	void FMODSound::Pause() {
		FMOD_CHECK_SOUND(return);
		bool isPaused = false;
		FMOD_CHECK_ERROR(m_Channel->getPaused(&isPaused), "Unable to check sound status", return);

		if (isPaused)
			return;

		FMOD_CHECK_ERROR(m_Channel->setPaused(true), "Unable to set sound status", return);
	}

	void FMODSound::Stop() {
		FMOD_CHECK_SOUND(return);

		m_Channel->stop();
	}

	void FMODSound::SetLoopCount(LoopCount loop) {
		FMOD_CHECK_SOUND(return);

		m_Channel->setLoopCount((int)loop);
	}

}