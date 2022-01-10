// Source: https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
#include "collision.h"

bool CheckCollisionAABB(AABB* a, AABB* b) {
    return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
        (a->min.y <= b->max.y && a->max.y >= b->min.y) &&
        (a->min.z <= b->max.z && a->max.z >= b->min.z);
}


bool CheckCollisionSphereAABB(Sphere* sphere, AABB* box) {
    // get box closest point to sphere center by clamping
    float x = std::max(box->min.x, std::min(sphere->center.x, box->max.x));
    float y = std::max(box->min.y, std::min(sphere->center.y, box->max.y));
    float z = std::max(box->min.z, std::min(sphere->center.z, box->max.z));

    // this is the same as isPointInsideSphere
    float distance = std::sqrt((x - sphere->center.x) * (x - sphere->center.x) +
        (y - sphere->center.y) * (y - sphere->center.y) +
        (z - sphere->center.z) * (z - sphere->center.z));

    return distance < sphere->radius;
}