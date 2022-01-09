#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Enemy : public Entity
	{
	public:
		Enemy(glm::vec3 pos);
		AABB* getCollisionBox();
		void ChangeDir();
		
		std::vector<glm::mat4> const& getModelMatrices();
		std::vector<Entity::Primitive> const& getPrimitives();

		std::vector<glm::vec3> directions;
		glm::vec3 translation;

		int currDir; // 0/1/2/3
	private:
		void Create();
		void InitCollisionBox();

		glm::vec3 pos;

		std::vector<Entity::Primitive> primitives;
		AABB collisionBox;
		AABB updatedCollisionBox;
	};
}