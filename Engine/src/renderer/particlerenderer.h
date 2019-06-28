#pragma once

#include "spriterenderer.h"
#include "game/particlesystem.h"

namespace prev {

	class ParticleRenderer : public Singleton<ParticleRenderer> {
		friend class Singleton<ParticleRenderer>;
	private:
		ParticleRenderer();
		~ParticleRenderer();
	public:
		void Render(const ParticleSystem & particleSystem);
	private:
		StrongHandle<PixelShader> m_PixelShader;
		unsigned int m_DrawGroupIndex;
	};

}
