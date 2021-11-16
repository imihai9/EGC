#pragma once

#include "components/simple_scene.h"
#include "entity.h"
#include "space.h"
#include "collision.h"

class Enemy : public Entity
{
public:
	Enemy(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();
	struct CircleCB* getCollisionBox(); // updates and returns the collision box

	float smallSquareSize;
	float largeSquareSize;
	float initial_tx, initial_ty;

	// Data for different renders
	struct EnemyData {
		glm::vec2 initialPos;
		glm::vec2 currentPos;
		float rotationAngle;
		float speedMultiplier;
		CircleCB collisionBox;
	};
private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;

	std::vector<glm::vec3> leftCorners;
};
