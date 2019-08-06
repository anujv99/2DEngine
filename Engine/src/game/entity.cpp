#include "pch.h"
#include "entity.h"

namespace prev {

	Entity::Entity(entityx::EntityX * ex) {
		m_Entity = ex->entities.create();
	}

	Entity::~Entity() {
		m_Entity.destroy();
	}

}