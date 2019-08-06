#pragma once

#include "entity.h"

#include "common/handledobject.h"

namespace prev {

	class ECS : public HandledObject<ECS> {
	public:
		ECS();
		~ECS();

		StrongHandle<Entity> CreateEntity();

		void Update(float deltaTime);

		template<typename T>
		void AddSystem() {
			m_EntitySystem.systems.add<T>();
			m_EntitySystem.systems.configure();
		}

	private:
		entityx::EntityX m_EntitySystem;
	};

}
