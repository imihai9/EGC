#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/entity.h"
#include "lab_m1/Tema2/collision.h"

namespace tema2 {
	class Projectile : public Entity
	{
	public:
		Projectile(glm::vec3 pos, glm::vec3 dir);
		Sphere* getCollisionBox();
		void ChangeDir();

		std::vector<Entity::Primitive> const& getPrimitives();
		glm::vec3 translation;
		glm::vec3 direction;
		float speed;
		float remainingTime;

	private:
		void Create();
		void InitCollisionBox();

		glm::vec3 pos;
		float size;
		std::vector<Entity::Primitive> primitives;
		Sphere collisionBox;
		Sphere updatedCollisionBox;
	};
}