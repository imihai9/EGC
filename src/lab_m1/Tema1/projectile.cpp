#include "projectile.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

Projectile::Projectile(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    this->size = 15.f;
    this->color = glm::vec3(0, 0, 0);

    modelMatrix = glm::mat3(1);
}

void Projectile::Create() {
    // Initial position; will be translated to the center of player's position when shooting occurs
    glm::vec3 leftCorner = glm::vec3(logicSpace.x - size / 2, logicSpace.y - size / 2, 0);
    Mesh* square = object::CreateSquare("projectile_rect", leftCorner, size, color, true);
    meshes.push_back(square);

    collisionBox = {
        // OBSOLETE - TO DELETE!! TODO
       // (float)(this->size * sqrt(2)),
       // glm::vec2(leftCorner.x + size / 2, leftCorner.y + size / 2)
    };
}


vector<Mesh*> const& Projectile::getMeshes() {
    return meshes;
}

// updates and returns the collision box
struct CircleCB* Projectile::getCollisionBox() {
    return &collisionBox;
}