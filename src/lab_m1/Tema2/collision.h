#pragma once

#include "core/engine.h"

// Rectangle Collision Box
struct AABB {
	glm::vec3 min; // minX, minY, minZ
	glm::vec3 max; // maxX, maxY, maxZ
};

bool CheckCollisionAABB (AABB *aabb1, AABB *aabb2);

