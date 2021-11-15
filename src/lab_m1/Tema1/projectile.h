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

private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;
	float size;
	glm::vec3 color;

	CircleCB collisionBox;
};
