#include "lab_m1/Tema2/projectile.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

Projectile::Projectile(glm::vec3 pos, glm::vec3 dir) {
    modelMatrix = glm::mat4(1); // global model matrix
    this->pos = pos;
    this->direction = dir;
    this->translation = glm::vec3(0);
    this->speed = 8.f;
    this->remainingTime = 0.5f;

    Create();
    InitCollisionBox();
}

void Projectile::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    matrix = glm::scale(matrix, glm::vec3(0.1f));
    primitives.push_back({ "sphere" , matrix });
}

std::vector<Entity::Primitive> const& Projectile::getPrimitives() {
    return primitives;
}

void Projectile::InitCollisionBox() {
    collisionBox.center = pos;
    collisionBox.radius = 1.f * 0.1f * 0.5f; // original sphere size * scale * half
    updatedCollisionBox.radius = collisionBox.radius;
}

// Updates and returns the collision box
Sphere* Projectile::getCollisionBox() {
    updatedCollisionBox.center = collisionBox.center + translation;
    return &updatedCollisionBox;
}
