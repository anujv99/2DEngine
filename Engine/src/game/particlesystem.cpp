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

		m_NumParticlesPerEmission		= 1u;
		m_Cooldown						= 0.0f;
		m_ElapsedTime					= 0.0f;

		m_LifeSpan						= 2.0f;
		m_LifeSpanVaraince				= 0.1f;

		m_StartPosition					= Vec2(0.0f);
		m_StartPositionVariance			= Vec2(0.0f);

		m_StartScale					= Vec2(1.0f);
		m_StartScaleVariance			= Vec2(0.1f);
		m_EndScale						= Vec2(0.0f);
		m_EndScaleVariance				= Vec2(0.0f);

		m_Velocity						= Vec2(0.0f);
		m_VelocityVariance				= Vec2(0.1f);

		m_Accel							= Vec2(0.0f);

		m_StartAngle					= 0.0f;
		m_StartAngleVariance			= 0.0f;
		m_RotationVelocity				= 0.0f;
		m_RotationVelocityVariance		= 0.0f;

		m_StartColor					= Vec3(1.0f);
		m_StartColorVariance			= Vec3(0.0f);
		m_EndColor						= Vec3(1.0f);
		m_EndColorVariance				= Vec3(0.0f);

		m_StartAlpha					= 1.0f;
		m_StartAlphaVariance			= 0.0f;
		m_EndAplha						= 1.0f;
		m_EndAplhaVariance				= 0.0f;

		std::srand((int)Timer::GetTime() * 321651320);
	}

	ParticleSystem::~ParticleSystem() {

	}

	void ParticleSystem::Update() {
		m_ElapsedTime += Timer::GetDeltaTime();
		if (m_Simulate && (m_Particles.size() < m_MaxNumParticles) && m_ElapsedTime >= m_Cooldown) {
			for (unsigned int i = 0; i < m_NumParticlesPerEmission; i++)
  				AddNewParticle();
			m_ElapsedTime = 0.0f;
		}
		UpdateAndRemoveParticles();
	}

	void ParticleSystem::AddParticle(const Particle & particle) {
		m_Particles.push_back(particle);
	}

	void ParticleSystem::Gui() {
		ImGui::Begin("Particle Settings", (bool *)0, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::Button("Clear")) {
			m_Particles.clear();
		}
		ImGui::Separator();
		ImGui::DragFloat("Life Span", &m_LifeSpan, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Life Span Variance", &m_LifeSpanVaraince, 0.1f, 0.0f, 100.0f);
		ImGui::DragFloat("Cooldown", &m_Cooldown, 0.1f, 0.0f, 1000.0f);
		ImGui::DragInt("Particles Per Emission", (int *)&m_NumParticlesPerEmission, 1, 1, 100);
		ImGui::Separator();
		ImGui::DragFloat2("Position", &m_StartPosition[0], 0.01f);
		ImGui::DragFloat2("Position Variance", &m_StartPositionVariance[0], 0.01f);
		ImGui::Separator();
		ImGui::DragFloat2("Start Scale", &m_StartScale[0], 0.01f);
		ImGui::DragFloat2("End Scale", &m_EndScale[0], 0.01f);
		ImGui::DragFloat2("Start Scale Variance", &m_StartScaleVariance[0], 0.01f);
		ImGui::DragFloat2("End Scale Variance", &m_EndScaleVariance[0], 0.01f);
		ImGui::Separator();
		ImGui::DragFloat2("Velocity", &m_Velocity[0], 0.01f);
		ImGui::DragFloat2("Velocity Variance", &m_VelocityVariance[0], 0.01f);
		ImGui::DragFloat2("Acceleration", &m_Accel[0], 0.01f);
		ImGui::Separator();
		ImGui::ColorEdit3("Start Color", &m_StartColor[0]);
		ImGui::ColorEdit3("End Color", &m_EndColor[0]);
		ImGui::DragFloat("Start Alpha", &m_StartAlpha, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("End Alpha", &m_EndAplha, 0.001f, 0.0f, 1.0f);
		ImGui::End();
	}

	bool ParticleSystem::UpdateSingleParticle(Particle & particle) {
		return particle.Update(Timer::GetDeltaTime());
	}

	void ParticleSystem::AddNewParticle() {
		Particle partice = {};

		partice.LifeLength = GenerateLifeSpan();
		partice.Position = GenerateVec2(m_StartPosition, m_StartPositionVariance);
		partice.Velocity = GenerateVec2(m_Velocity, m_VelocityVariance);
		partice.StartScale = GenerateVec2(m_StartScale, m_StartScaleVariance, false);
		partice.EndScale = GenerateVec2(m_EndScale, m_EndScaleVariance, false);
		partice.Acceleration = m_Accel;
		partice.StartColor = GenerateVec3(m_StartColor, m_StartColorVariance, false);
		partice.EndColor = GenerateVec3(m_EndColor, m_EndColorVariance, false);
		partice.StartAlpha = GenerateFloat(m_StartAlpha, m_StartAlphaVariance, false);
		partice.EndAlpha = GenerateFloat(m_EndAplha, m_EndAplhaVariance, false);
		partice.StartAngle = GenerateFloat(m_StartAngle, m_StartAngleVariance);
		partice.RotationVelocity = GenerateFloat(m_RotationVelocity, m_RotationVelocityVariance);

		partice.CurrentScale = partice.StartScale;
		partice.CurrentAlpha = partice.StartAlpha;
		partice.CurrentColor = partice.StartColor;

		AddParticle(partice);
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

	float ParticleSystem::GenerateRandom() {
		float ran = (float)std::rand() / (float)RAND_MAX;
		return (ran * 2.0f) - 1.0f;
	}

	float ParticleSystem::GenerateLifeSpan() {
		float life = m_LifeSpan + (m_LifeSpanVaraince * GenerateRandom());
		if (life <= 0.0f) {
			return m_LifeSpan;
		}
		return life;
	}

	float ParticleSystem::GenerateFloat(float start, float variance, bool canBeNegative) {
		float val = start + (variance * GenerateRandom());
		if (!canBeNegative) {
			val = start;
		}
		return val;
	}

	prev::Vec2 ParticleSystem::GenerateVec2(Vec2 start, Vec2 variance, bool canBeNegative) {
		Vec2 val = start + (variance * Vec2(GenerateRandom(), GenerateRandom()));
		if (!canBeNegative) {
			if (val.x < 0) {
				val.x = start.x;
			}
			if (val.y < 0) {
				val.y = start.y;
			}
		}
		return val;
	}

	prev::Vec3 ParticleSystem::GenerateVec3(Vec3 start, Vec3 variance, bool canBeNegative) {
		Vec3 val = start + (variance * Vec3(GenerateRandom(), GenerateRandom(), GenerateRandom()));
		if (!canBeNegative) {
			if (val.x < 0) {
				val.x = start.x;
			}
			if (val.y < 0) {
				val.y = start.y;
			}
			if (val.z < 0) {
				val.z = start.z;
			}
		}
		return val;
	}

}
