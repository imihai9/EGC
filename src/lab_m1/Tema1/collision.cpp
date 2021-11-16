/* Sources:
https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
*/
#include "collision.h"

int sgn (float val) {
    return (0.f < val) - (val < 0.f);
}

glm::vec2 CheckCollisionRectCircle(RectCB *rectCB, CircleCB *circleCB) {
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(rectCB->width / 2.0f, rectCB->height / 2.0f);
    glm::vec2 aabb_center(
        rectCB->pos_x + aabb_half_extents.x,
        rectCB->pos_y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = circleCB->center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    // add clamped value to AABB_center and we get the point from rectangle closest to circle
    glm::vec2 closest = aabb_center + clamped;

    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - circleCB->center;
    //return glm::length(difference) < circleCB->radius;

    float penetrationX = 0;
    float penetrationY = 0;

    float difference_length = glm::length(difference);
    if (difference_length < circleCB->radius && difference_length > 0) {
        penetrationX = -sgn(difference.x) * (float)((circleCB->radius - difference_length) * (glm::abs(difference.x) / difference_length));
        penetrationY = -sgn(difference.y) * (float)((circleCB->radius - difference_length) * (glm::abs(difference.y) / difference_length));
        return glm::vec2(penetrationX, penetrationY);
    }

    return glm::vec2(0);
}

// Returns penetration length if collision occured, 0 otherwise
glm::vec2 CheckCollisionRectCircleInside(RectCB* rectCB, CircleCB* circleCB) {
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(rectCB->width / 2.0f, rectCB->height / 2.0f);
    glm::vec2 aabb_center(
        rectCB->pos_x + aabb_half_extents.x,
        rectCB->pos_y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = circleCB->center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);


    // horizontal collision: -----
    // closest point to circle (on rectangle)
    glm::vec2 v1 = aabb_center + glm::vec2(sgn(clamped.x) * aabb_half_extents.x, clamped.y);

    float penetration_x = 0;
    float penetration_y = 0;
    bool collided = false;
    
    difference = v1 - circleCB->center;
    if (glm::length(difference) < circleCB->radius) {
        penetration_x = - sgn(difference.x) * (circleCB->radius - std::abs(difference.x));
        collided = true;
    }

    // vertical collision: -----
    glm::vec2 v2 = aabb_center + glm::vec2(clamped.x, clamped.y / abs(clamped.y) * aabb_half_extents.y);
    difference = v2 - circleCB->center;

    if (glm::length(difference) < circleCB->radius) {
        penetration_y = - sgn(difference.y) * (circleCB->radius - std::abs(difference.y));
        collided = true;
    }
    
    if (collided)
        return glm::vec2(penetration_x, penetration_y);
    else
        return glm::vec2(0); // no collision occurred
}

bool CheckCollisionCircleCircle(CircleCB* circleCB1, CircleCB* circleCB2) {
    float dx = (circleCB1->center.x) - (circleCB2->center.x);
    float dy = (circleCB1->center.y) - (circleCB2->center.y);

    float distance = glm::sqrt(dx * dx + dy * dy);

    if (distance < circleCB1->radius + circleCB2->radius)
        return true;
    else
        return false;
}