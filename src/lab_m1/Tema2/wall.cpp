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
}

void Wall::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    primitives.push_back({ "blue_cube" , matrix});
  //  collisionBox.center = pos;
  //  collisionBox.size = glm::vec3(2);
}

std::vector<Entity::Primitive> const& Wall::getPrimitives() {
    return primitives;
}

// Returns the collision box
AABB* Wall::getCollisionBox() {
    // A wall is a cube of size 
    collisionBox.minX = pos.x - 1;
    collisionBox.maxX = pos.x + 1;
    collisionBox.minY = pos.y - 1;
    collisionBox.maxY = pos.y + 1;
    collisionBox.minZ = pos.z - 1;
    collisionBox.maxZ = pos.z + 1;

    //cout << "w x: " << collisionBox.minX << ' ' << collisionBox.maxX << endl;
  //  cout << "w y: " << collisionBox.minY << ' ' << collisionBox.maxY << endl;
   // cout << "w z: " << collisionBox.minZ << ' ' << collisionBox.maxZ << endl;

    return &collisionBox;
}
