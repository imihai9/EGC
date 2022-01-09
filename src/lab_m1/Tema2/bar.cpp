#include "lab_m1/Tema2/bar.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

Bar::Bar(glm::vec3 pos) { 
    modelMatrix = glm::mat4(1); // global model matrix
    this->pos = pos;
    Create();
}

void Bar::Create() {
    glm::mat4 matrix = glm::translate(glm::mat4(1), pos);
    primitives.push_back({ "blue_cube" , matrix });
}

std::vector<Entity::Primitive> const& Bar::getPrimitives() {
    return primitives;
}
