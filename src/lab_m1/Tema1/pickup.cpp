#include "lab_m1/Tema1/pickup.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

// Health pickup

// A single object will be instantiated (should be a Singleton)
// Data for different renders -> in PickupData struct
Pickup::Pickup(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    modelMatrix = glm::mat3(1);
}

void Pickup::Create() {
    Mesh* square;
    square = object::CreateSquare("pickup_rect", glm::vec3(0), 40.f, glm::vec3(0.20f, 0.42f, 0.19f), false);
    meshes.push_back(square);
}

vector<Mesh*> const& Pickup::getMeshes() {
    return meshes;
}
