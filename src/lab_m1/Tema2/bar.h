#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Bar : public Entity
	{
	public:
		Bar(glm::vec3 pos);
		std::vector<Entity::Primitive> const& getPrimitives();

	private:
		void Create();

		glm::vec3 pos;
		std::vector<Entity::Primitive> primitives;
	};
}