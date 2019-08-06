#pragma once

#include "entityx/entityx.h"

#include "renderer/renderer.h"

#include "predefinedcomponents.h"

namespace prev {

	class RendererSystem : public entityx::System<RendererSystem> {
	public:
		void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
			es.each<GameObject, Color, Renderable>([dt](entityx::Entity entity, GameObject & object, Color & color, Renderable & renderable) -> void {
				Sprite s;
				s.Color = *(SpriteColor *)&color;
				s.Dimension = object.Dimension;
				s.Position = object.Position.xy();
				s.Depth = object.Position.z;
				s.Rotation = object.Rotation;

				Renderer::Ref().Submit(s, nullptr, renderable.VertShader, renderable.PixShader);
			});
		}
	};

}
