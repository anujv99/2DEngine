#pragma once

#include <Box2D/Box2D.h>

#include "math/vec2.h"

namespace prev {

	class Box2DManager : public Singleton<Box2DManager> {
		friend class Singleton<Box2DManager>;
	private:
		Box2DManager();
		~Box2DManager();
	public:
		void Update();

		b2World * CreateBox2DWorld(const std::string & name, Vec2 gravity);
	private:
		std::unordered_map<uint32_t, b2World *> m_CollisionWorlds;
	};

}
