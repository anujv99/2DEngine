#pragma once

#include "fmodincludes.h"
#include "audio/sound.h"

namespace prev {

	class FMODSound : public Sound {
		friend class FMODAudio;
	private:
		FMODSound(const std::string & fileName, FMOD::Studio::System * system);
		~FMODSound();

		bool Initialize(const std::string & fileName, FMOD::Studio::System * system);
	public:
		// Inherited via Sound
		virtual void Play() override;
		virtual void Pause() override;
		virtual void Stop() override;
		virtual void SetLoopCount(LoopCount loop) override;
	private:
		FMOD::Sound * m_Sound;
		FMOD::Channel * m_Channel;
		std::string m_FileName;
		bool m_IsLoaded;
	};

}
