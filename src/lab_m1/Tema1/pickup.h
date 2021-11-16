#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "lab_m1/Tema1/space.h"
#include "lab_m1/Tema1/collision.h"
#include "lab_m1/Tema1/enemy.h"

// Health pickup
class Pickup : public Entity
{
public:
	Pickup(LogicSpace logicSpace);
	void Create();

	std::vector<Mesh*> const& getMeshes();

	float smallSquareSize;
	float largeSquareSize;
	float initial_tx, initial_ty;

	// Data for different renders
	struct PickupData {
		glm::vec2 pos;
		RectCB collisionBox;
		bool type;	// 0 = health, 1 = firerate
	};
private:
	std::vector<Mesh*> meshes;
	LogicSpace logicSpace;
};
