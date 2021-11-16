#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "lab_m1/Tema1/space.h"
#include "lab_m1/Tema1/collision.h"

class Bar : public Entity
{
public:
	Bar(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();

	Mesh* currHealth;
	Mesh* maxHealth;
private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;
	glm::vec3 leftCorner;
	glm::vec2 size;
	glm::vec3 color;
};
