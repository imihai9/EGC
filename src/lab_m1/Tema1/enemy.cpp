#include "enemy.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

Enemy::Enemy(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    this->smallSquareSize = 20;
    this->largeSquareSize = 40;

    // Initial translation (in order to have enemy center in origin)
    float x = -30, y = -25;
    leftCorners = { glm::vec3(x, y, 0),
                  glm::vec3(x + 40, y, 0),
                  glm::vec3(x + 10, y + 10, 0) };

    modelMatrix = glm::mat3(1);
}

void Enemy::Create() {
    Mesh* square;
    square = object::CreateSquare("enemy_rect2", leftCorners[2], largeSquareSize, glm::vec3(0.66f, 0, 0.07f), true);
    meshes.push_back(square);
    square = object::CreateSquare("enemy_rect0", leftCorners[0], smallSquareSize, glm::vec3(0.48f, 0.18f, 0.18f), true);
    meshes.push_back(square);
    square = object::CreateSquare("enemy_rect1", leftCorners[1], smallSquareSize, glm::vec3(0.48f, 0.18f, 0.18f), true);
    meshes.push_back(square);

    collisionBox = {
        // OBSOLETE - TO DELETE!! TODO
       // (float)(this->size * sqrt(2)),
       // glm::vec2(leftCorner.x + size / 2, leftCorner.y + size / 2)
    };
}


vector<Mesh*> const& Enemy::getMeshes() {
    return meshes;
}

// updates and returns the collision box
struct CircleCB* Enemy::getCollisionBox() {
    return &collisionBox;
}