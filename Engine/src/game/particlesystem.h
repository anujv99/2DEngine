#pragma once

#include "particle.h"

namespace prev {

	class ParticleSystem : public HandledObject<ParticleSystem> {
		friend class Renderer;
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
	private:
		//Generate Random Number Between -1.0f and  1.0f
		float GenerateRandom();
		float GenerateLifeSpan();
		float GenerateFloat(float start, float variance, bool canBeNegative = true);
		Vec2 GenerateVec2(Vec2 start, Vec2 variance, bool canBeNegative = true);
		Vec3 GenerateVec3(Vec3 start, Vec3 variance, bool canBeNegative = true);
	public:
		std::vector<Particle> m_Particles;
		unsigned int m_MaxNumParticles;
		bool m_Simulate;
		
		//Number of particles
		unsigned int m_NumParticlesPerEmission;

		//Cooldown
		float m_Cooldown;
		float m_ElapsedTime;

		//Life
		float m_LifeSpan;
		float m_LifeSpanVaraince;

		//Position
		Vec2 m_StartPosition;
		Vec2 m_StartPositionVariance;

		//Scale
		Vec2 m_StartScale;
		Vec2 m_StartScaleVariance;
		Vec2 m_EndScale;
		Vec2 m_EndScaleVariance;

		//Speed
		Vec2 m_Velocity;
		Vec2 m_VelocityVariance;

		//Acceleration
		Vec2 m_Accel;

		//Attractor
		Vec2 m_AttractorPosition;
		float m_AttractorStrenght;

		//Angle :- Not using right now
		float m_StartAngle;
		float m_StartAngleVariance;
		float m_RotationVelocity;
		float m_RotationVelocityVariance;

		//Color
		Vec3 m_StartColor;
		Vec3 m_StartColorVariance;
		Vec3 m_EndColor;
		Vec3 m_EndColorVariance;
		float m_StartAlpha;
		float m_StartAlphaVariance;
		float m_EndAlpha;
		float m_EndAlphaVariance;
	};

}
