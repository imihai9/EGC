#include "lab_m1/Tema2/enemy.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

Enemy::Enemy(glm::vec3 pos) { // pos = cube center position
    modelMatrix = glm::mat4(1); // global model matrix
    this->pos = pos;
    this->translation = glm::vec3(0);
    this->directions = {
        glm::vec3(+1, 0, 0), // OX +
        glm::vec3(0, 0, +1), // OZ +
        glm::vec3(-1, 0, 0), // OX -
        glm::vec3(0, 0, -1)  // OZ -
    };

    this->size = glm::vec3(1.2f, 2, 1.2f);
    this->currDir = 0;
    this->dmgCaused = 1.f;
    this->speed = 1.f;

    Create();
    InitCollisionBox();
}

void Enemy::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    matrix = glm::scale(matrix, size * 0.5f ); // divide by 2 (original cube size)
    primitives.push_back({ "red_cube" , matrix });

    glm::mat4 head_matrix = glm::translate(glm::mat4(1), pos);
   // head_matrix = glm::scale(head_matrix, glm::vec3(1.f));

    primitives.push_back({ "sphere", head_matrix });
}

void Enemy::ChangeDir() {
    currDir = (currDir + 1) % 4;
}

void Enemy::InitCollisionBox() {
    collisionBox.min = pos - size * 0.5f;
    collisionBox.max = pos + size * 0.5f;
}

std::vector<Entity::Primitive> const& Enemy::getPrimitives() {
    return primitives;
}

// Returns the collision box
AABB* Enemy::getCollisionBox() {
    updatedCollisionBox.min = collisionBox.min + translation;
    updatedCollisionBox.max = collisionBox.max + translation;
    return &updatedCollisionBox;
}
