#include "lab_m1/Tema2/wall.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

// "wall" = cube
// All walls have the same size

Wall::Wall(glm::vec3 pos) { // pos = cube center position
    modelMatrix = glm::mat4(1); // global model matrix
    this->pos = pos;
    Create();
    InitCollisionBox();
}

void Wall::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    primitives.push_back({ "blue_cube" , matrix});
}

void Wall::InitCollisionBox() {
    // A wall is a cube of size 
    // collisionBox.center = pos;
    // collisionBox.size = glm::vec3(2);
    collisionBox.min = glm::vec3(pos.x - 1, pos.y - 1, pos.z - 1);
    collisionBox.max = glm::vec3(pos.x + 1, pos.y + 1, pos.z + 1);
}

std::vector<Entity::Primitive> const& Wall::getPrimitives() {
    return primitives;
}

// Returns the collision box
AABB* Wall::getCollisionBox() {
    return &collisionBox;
}
