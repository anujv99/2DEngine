#include "pch.h"
#include "fmodaudio.h"

#include "fmodsound.h"

#if defined(ENGINE_DEBUG)
	#define FMOD_CHECK_INIT(...) if (!m_IsInitialized) { LOG_ERROR("FMOD not initialized. Unable to call function : {}", __FUNCTION__); __VA_ARGS__; }
#else defined(ENGINE_REALEASE) || defined(ENGINE_DIST)
	#define FMOD_CHECK_INIT(...)
#endif

namespace prev {

	FMODAudio::FMODAudio() : m_System(nullptr), m_IsInitialized(false) {
		m_IsInitialized = Initialize();

		if (m_IsInitialized == false) {
			LOG_ERROR("Unable to initialize FMOD");
		}
	}

	FMODAudio::~FMODAudio() {
		m_System->release();
		m_IsInitialized = false;
	}

	bool FMODAudio::Initialize() {
		if (m_IsInitialized) return true;

		FMOD::System * coreSystem = nullptr;
		void * edd = nullptr;

		FMOD_CHECK_ERROR(FMOD::Studio::System::create(&m_System), "Unable to create FMOD system", return false);
		FMOD_CHECK_ERROR(m_System->getCoreSystem(&coreSystem), "Unable to get FMOD core system", return false);
		FMOD_CHECK_ERROR(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0), "Unable to set FMOD software format and speaker mode", return false);
		FMOD_CHECK_ERROR(m_System->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, edd), "Unable to initialize FMOD system", return false);

		return true;
	}

	prev::StrongHandle<prev::Sound> FMODAudio::LoadSound(const std::string & fileName) {
		FMOD_CHECK_INIT();

		FMODSound * sound = new FMODSound(fileName, m_System);
		return sound;
	}

	void FMODAudio::Update() {
		m_System->update();
	}

}
