#include "lab_m1/Tema1/obstacle.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/bar.h"
#include <core/gpu/mesh.h>
using namespace std;

Bar::Bar(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    this->color = glm::vec3(0.20f, 0.42f, 0.19f);
    modelMatrix = glm::mat3(1);
}

void Bar::Create() {
    // initially in origin; will be translated
    currHealth = object::CreateRectangle("obst_rect0", glm::vec3(0), 100, 20, color, true);
    maxHealth = object::CreateRectangle("obst_rect1", glm::vec3(0), 100, 20, color, false);

   
    meshes.push_back(currHealth);
    meshes.push_back(maxHealth);
}


vector<Mesh*> const& Bar::getMeshes() {
    return meshes;
}