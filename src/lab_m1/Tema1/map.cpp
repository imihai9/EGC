#include "lab_m1/Tema1/map.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

Map::Map(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    modelMatrix = glm::mat3(1);

    float tx = logicSpace.width * 0.1f;
    float ty = logicSpace.width * 0.1f;

    collisionBox = {
        logicSpace.x + tx / 2,
        logicSpace.y + ty / 2,
        logicSpace.width - tx,
        logicSpace.height - ty
    };
}

void Map::Create() {
    glm::vec3 corner = glm::vec3(logicSpace.x, logicSpace.y, 0);

    float tx = logicSpace.width * 0.1f;
    float ty = logicSpace.width * 0.1f;
    
    glm::vec3 rect1_color = glm::vec3(0.2f, 0.5f, 1.f);
    glm::vec3 rect2_color = glm::vec3(0.75f, 0.75f, 0.76f);

    Mesh* rect1 = object::CreateRectangle("map_rect0", corner, logicSpace.width, logicSpace.height, rect1_color, true);
    // concentrical rectangle
    Mesh* rect2 = object::CreateRectangle("map_rect1", corner + glm::vec3(tx/2, ty/2, 0), logicSpace.width - tx, logicSpace.height - ty,
       rect2_color, true);

    meshes.push_back(rect2);
    meshes.push_back(rect1);
}

vector<Mesh*> const& Map::getMeshes() {
    return meshes;
}

// returns the collision box
struct RectCB* Map::getCollisionBox() {
    return &collisionBox;
}