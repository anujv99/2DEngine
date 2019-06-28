#pragma once

#include "particle.h"

namespace prev {

	class ParticleSystem : HandledObject<ParticleSystem> {
		friend class ParticleRenderer;
	public:
		ParticleSystem(unsigned int numParticles);
		~ParticleSystem();

		void Update();
		void AddParticle(const Particle & particle);
		void Gui();
	private:
		bool UpdateSingleParticle(Particle & particle);
		void AddNewParticle();
		void UpdateAndRemoveParticles();

		Vec2 GenerateVelocity(const Vec2 vel);
		Vec2 GeneratePosition(const Vec2 pos);
		float GenerateScale(const float scale);
	private:
		std::vector<Particle> m_Particles;
		unsigned int m_MaxNumParticles;
		bool m_Simulate;
		
		Vec2 m_Position;
		Vec2 m_VelocityVariance;
		Vec2 m_PositionVariance;
		Vec4 m_Color;
		float m_Scale;
		float m_ScaleVariace;
		float m_ScaleDecay;
		float m_Life;
	};

}
