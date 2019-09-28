#pragma once

#include "fmodincludes.h"

#include "audio/sound.h"

namespace prev {

	class FMODAudio : public Singleton<FMODAudio> {
		friend class Singleton<FMODAudio>;
	private:
		FMODAudio();
		~FMODAudio();

		bool Initialize();
	public:
		StrongHandle<Sound> LoadSound(const std::string & fileName);

		void Update();
	private:
		FMOD::Studio::System * m_System;

		bool m_IsInitialized;
	};

}
