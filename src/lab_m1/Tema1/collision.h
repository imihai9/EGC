#pragma once

#include "core/engine.h"

// Rectangle Collision Box
struct RectCB { 
	float pos_x;
	float pos_y;
	float width;
	float height;
};

// Circle Collision Box
struct CircleCB {
	float radius;
	glm::vec2 center;
};

glm::vec2 CheckCollisionRectCircle(RectCB* rectCB, CircleCB* circleCB);
glm::vec2 CheckCollisionRectCircleInside(RectCB* rectCB, CircleCB* circleCB);
bool CheckCollisionCircleCircle(CircleCB* circleCB1, CircleCB* circleCB2);
// bool CheckCollisionRectRect();

