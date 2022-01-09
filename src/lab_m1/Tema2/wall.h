#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Wall : public Entity
	{
	public:
		Wall(glm::vec3 pos);
		void Create();
		void InitCollisionBox();

		std::vector<glm::mat4> const& getModelMatrices();
		std::vector<Entity::Primitive> const& getPrimitives();

		AABB* getCollisionBox();

	private:
		glm::vec3 pos;
		std::vector<Entity::Primitive> primitives;
		AABB collisionBox;
	};
}