#pragma once

#include "core/engine.h"

// Rectangle Collision Box
struct AABB {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
};

bool CheckCollisionAABB (AABB *aabb1, AABB *aabb2);

