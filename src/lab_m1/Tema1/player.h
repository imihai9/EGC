#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "space.h"
#include "collision.h"

class Player : public Entity
{
public:
	Player(LogicSpace logicSpace);
	void Create();
	
	float translateX;
	float translateY;

	glm::vec2 lastTranslation; // used for undoing translation on collision detection
	std::vector<Mesh*> const& getMeshes();
	struct CircleCB* getCollisionBox(); // updates and returns the collision box

private:
	int meshCount; // number of meshes used to represent the player model
	std::vector<glm::vec2> circleCenters;
	std::vector<Mesh*> meshes;

	float smallCircleRadius = 10;
	float largeCircleRadius = 20;

	CircleCB collisionBox;
};
