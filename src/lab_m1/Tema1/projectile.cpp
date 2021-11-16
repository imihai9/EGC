#include "lab_m1/Tema1/projectile.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

// A single object will be instantiated (should be a Singleton)
// Data for different renders -> in ProjData struct
Projectile::Projectile(LogicSpace logicSpace) {
    this->logicSpace = logicSpace;
    this->size = 15.f;
    this->color = glm::vec3(0, 0, 0);

    modelMatrix = glm::mat3(1);
}

void Projectile::Create() {
    // Initial position; will be translated to the center of player's position when shooting occurs
    glm::vec3 leftCorner = glm::vec3(logicSpace.x - size / 2, logicSpace.y - size / 2, 0);
    Mesh* square = object::CreateSquare("projectile_rect", leftCorner, size, color, true);
    meshes.push_back(square);
}

vector<Mesh*> const& Projectile::getMeshes() {
    return meshes;
}