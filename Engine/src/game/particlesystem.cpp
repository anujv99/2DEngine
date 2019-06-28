#include "pch.h"
#include "particlesystem.h"

#include "utils/input.h"
#include "imgui.h"
#include "math/vecconversion.h"

namespace prev {

	ParticleSystem::ParticleSystem(unsigned int numParticles) {
		m_Particles.reserve(numParticles);
		m_Simulate = true;
		m_MaxNumParticles = numParticles;
		m_Position = Vec2(400.0f);
		m_PositionVariance = Vec2(0.0f);
		m_VelocityVariance = Vec2(1.0f);
		m_Color = Vec4(0.1f, 0.9f, 0.3f, 0.5f);
		m_ScaleVariace = 10.0f;
		m_Scale = 50.0f;
		m_ScaleDecay = 0.0f;
		m_Life = 1.0f;
		std::srand((int)Timer::GetTime() * 321651320);
	}

	ParticleSystem::~ParticleSystem() {

	}

	void ParticleSystem::Update() {
		if (m_Simulate && (m_Particles.size() < m_MaxNumParticles)) {
			AddNewParticle();
		}
		UpdateAndRemoveParticles();
	}

	void ParticleSystem::AddParticle(const Particle & particle) {
		m_Particles.push_back(particle);
	}

	void ParticleSystem::Gui() {
		ImGui::Begin("Particle Testing");
		if (ImGui::Button("Clear")) {
			m_Particles.clear();
			m_Particles.reserve(m_MaxNumParticles);
		}
		ImGui::DragFloat2("Position", &m_Position[0], 1);
		ImGui::DragFloat2("Position Variance", &m_PositionVariance[0], 0.1f);
		ImGui::ColorEdit4("Color", &m_Color[0]);
		ImGui::DragFloat2("Velocity Variance", &m_VelocityVariance[0], 0.1f);
		ImGui::DragFloat("Start Scale", &m_Scale, 0.1f);
		ImGui::DragFloat("Scale Variance", &m_ScaleVariace, 0.1f);
		ImGui::DragFloat("Scale Decay", &m_ScaleDecay, 0.1f);
		ImGui::DragFloat("Life", &m_Life, 0.1f);
		ImGui::End();
	}

	bool ParticleSystem::UpdateSingleParticle(Particle & particle) {
		particle.ElapsedTime += Timer::GetDeltaTime();
		particle.Position += particle.Velocity;

		float factor = particle.LifeLength - particle.ElapsedTime;
		factor /= particle.LifeLength;

		particle.Scale -= m_ScaleDecay;

		if (particle.Scale < 0.0f)
			return false;

		return particle.ElapsedTime < particle.LifeLength;
	}

	void ParticleSystem::AddNewParticle() {
		Particle particle;
		particle.LifeLength = m_Life;
		//particle.Position = ToVec2(Input::Ref().GetMousePosition());
		particle.Position = GeneratePosition(m_Position);
		particle.Scale = GenerateScale(m_Scale);
		particle.Velocity = GenerateVelocity(Vec2(4));
		particle.Color = m_Color;

		AddParticle(particle);
	}

	void ParticleSystem::UpdateAndRemoveParticles() {
		auto it = m_Particles.begin();
		while (it != m_Particles.end()) {
			if (!UpdateSingleParticle(*it)) {
				it = m_Particles.erase(it);
			} else {
				it++;
			}
		}
	}

	prev::Vec2 ParticleSystem::GenerateVelocity(const Vec2 vel) {
		float randX = (float)std::rand();
		randX /= (float)RAND_MAX;
		randX -= 0.5f;

		float randY = (float)std::rand();
		randY /= (float)RAND_MAX;
		randY -= 0.5f;

		Vec2 newVel = vel * Vec2(m_VelocityVariance.x * randX, m_VelocityVariance.y * randY);
		return newVel;
	}

	prev::Vec2 ParticleSystem::GeneratePosition(const Vec2 pos) {
		float randX = (float)std::rand();
		randX /= (float)RAND_MAX;
		randX -= 0.5f;

		float randY = (float)std::rand();
		randY /= (float)RAND_MAX;
		randY -= 0.5f;

		Vec2 newVel = pos + Vec2(m_PositionVariance.x * randX, m_PositionVariance.y * randY);
		return newVel;
	}

	float ParticleSystem::GenerateScale(const float scale) {
		float rand = (float)std::rand();
		rand /= (float)RAND_MAX;
		rand = rand * 2.0f - 1.0f;

		return scale + m_ScaleVariace * rand;
	}

}
