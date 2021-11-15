#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "space.h"
#include "collision.h"

class Map : public Entity
{
public:
	Map(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();
	struct RectCB* getCollisionBox(); // updates and returns the collision box

private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;
	RectCB collisionBox;
};
