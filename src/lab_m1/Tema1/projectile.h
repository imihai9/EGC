#pragma once

#include "components/simple_scene.h"
#include "entity.h"
#include "space.h"
#include "collision.h"

class Projectile : public Entity
{
public:
	Projectile(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();
	struct CircleCB* getCollisionBox(); // updates and returns the collision box

	float size;

	struct ProjectileData {
		glm::vec2 initialPos;
		float rotationAngle;
		float moveFactor;
	};
private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;

	glm::vec3 color;

	CircleCB collisionBox;
};
