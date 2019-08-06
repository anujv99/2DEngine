#include "pch.h"
#include "ecs.h"

namespace prev {

	ECS::ECS() {}

	ECS::~ECS() {}

	StrongHandle<Entity> ECS::CreateEntity() {
		return new Entity(&m_EntitySystem);
	}

	void ECS::Update(float deltaTime) {
		m_EntitySystem.systems.update_all(deltaTime);
	}

}