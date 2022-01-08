#include "collision.h"

bool CheckCollisionAABB(AABB* a, AABB* b) {
    return (a->minX <= b->maxX && a->maxX >= b->minX) &&
        (a->minY <= b->maxY && a->maxY >= b->minY) &&
        (a->minZ <= b->maxZ && a->maxZ >= b->minZ);
}