#include "lab_m1/Tema1/obstacle.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

int Obstacle::obstCount = 0;

Obstacle::Obstacle(LogicSpace logicSpace, glm::vec3 leftCorner, glm::vec2 size, glm::vec3 color) {
    this->logicSpace = logicSpace;
    this->leftCorner = leftCorner;
    this->size = size;
    this->color = color;

    modelMatrix = glm::mat3(1);

    obstCount++;
}

void Obstacle::Create() {
    Mesh* rect = object::CreateRectangle("obst_rect" + to_string(obstCount), leftCorner, size.x, size.y, color, true);
    meshes.push_back(rect);

    collisionBox = {
        leftCorner.x,
        leftCorner.y,
        size.x,
        size.y
    };
}


vector<Mesh*> const& Obstacle::getMeshes() {
    return meshes;
}

// returns the collision box
struct RectCB* Obstacle::getCollisionBox() {
    return &collisionBox;
}