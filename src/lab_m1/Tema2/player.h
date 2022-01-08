#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Player : public Entity
	{
	public:
		Player();
		void Create();

		float translateX;
		float translateY;

		std::vector<glm::mat4> const& getModelMatrices();
		std::vector<Entity::Primitive> const& getPrimitives();
		AABB* getCollisionBox();

	private:
		std::vector<Entity::Primitive> primitives;
		AABB collisionBox;
	};
}