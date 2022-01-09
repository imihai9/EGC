#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Player : public Entity
	{
	public:
		Player();
		AABB* getCollisionBox();

		glm::vec3 translation;

		std::vector<glm::mat4> const& getModelMatrices();
		std::vector<Entity::Primitive> const& getPrimitives();

	private:		
		void Create();
		void InitCollisionBox();

		std::vector<Entity::Primitive> primitives;
		AABB collisionBox;
		AABB updatedCollisionBox;
	};
}