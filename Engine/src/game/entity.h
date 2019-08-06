#pragma once

#include <entityx/entityx.h>

namespace prev {

	class Entity : public HandledObject<Entity> {
		friend class ECS;
		friend class StrongHandle<Entity>;
	private:
		Entity(entityx::EntityX * ex);
		~Entity();
	public:
		template<typename T, typename ... Args>
		void AddComponent(Args && ... args) {
			m_Entity.assign<T>(args ...);
		}

		template<typename T>
		T & GetComponent() {
			entityx::ComponentHandle<T, entityx::EntityManager> comp = m_Entity.component<T>();
			return *comp.get();
		}
	private:
		entityx::Entity m_Entity;
	};

}
