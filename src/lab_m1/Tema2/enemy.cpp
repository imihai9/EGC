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

    Create();
    InitCollisionBox();

    this->directions = { 
        glm::vec3(+1, 0, 0), // OX +
        glm::vec3(0, 0, +1),// OZ +
        glm::vec3(-1, 0, 0),// OX -
        glm::vec3(0, 0, -1)// OZ -
    };

    this->currDir = 0;
}

void Enemy::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    matrix = glm::scale(matrix, glm::vec3(0.7f / 2, 1, 0.7f / 2)); // divide by 5 on X,Z axis => 0.4f
    primitives.push_back({ "red_cube" , matrix });
}

void Enemy::ChangeDir() {
    currDir = (currDir + 1) % 4;
}

void Enemy::InitCollisionBox() {
    // A wall is a cube of size 
    // collisionBox.center = pos;
    // collisionBox.size = glm::vec3(0.4, 2, 0.4);
    collisionBox.min = glm::vec3(pos.x - 0.35f, pos.y - 1, pos.z - 0.35f);
    collisionBox.max = glm::vec3(pos.x + 0.35f, pos.y + 1, pos.z + 0.35f);
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
