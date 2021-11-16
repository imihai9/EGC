#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "lab_m1/Tema1/space.h"
#include "lab_m1/Tema1/collision.h"

class Projectile : public Entity
{
public:
	Projectile(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();

	float size;

	// Data for different renders
	struct ProjectileData {
		glm::vec2 initialPos;
		float rotationAngle;
		float moveFactor;
		CircleCB collisionBox;
	};
private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;

	glm::vec3 color;
};
