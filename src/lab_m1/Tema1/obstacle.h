#pragma once

#include "components/simple_scene.h"
#include "entity.h"
#include "space.h"
#include "collision.h"

class Obstacle : public Entity
{
public:
	Obstacle(LogicSpace logicSpace, glm::vec3 leftCorner, glm::vec2 size, glm::vec3 color);
	void Create();

	std::vector<Mesh*> const& getMeshes();
	struct RectCB* getCollisionBox(); // updates and returns the collision box

private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;
	static int obstCount;
	glm::vec3 leftCorner;
	glm::vec2 size;
	glm::vec3 color;

	RectCB collisionBox;
};
