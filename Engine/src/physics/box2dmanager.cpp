#include "pch.h"
#include "box2dmanager.h"

namespace prev {

	Box2DManager::Box2DManager() {

	}

	Box2DManager::~Box2DManager() {
		for (auto & world : m_CollisionWorlds) {
			delete world.second;
		}
		m_CollisionWorlds.clear();
	}

	void Box2DManager::Update() {
		for (auto & world : m_CollisionWorlds) {
			world.second->Step(1.0f / 60.0f, 6, 2);
		}
	}

	b2World * Box2DManager::CreateBox2DWorld(const std::string & name, Vec2 gravity) {
		uint32_t hash = HashString(name);
		b2World * world = new b2World(gravity);
		m_CollisionWorlds.insert(std::make_pair(hash, world));
		return world;
	}

}