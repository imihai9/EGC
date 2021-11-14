#include "player.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"
#include <core/gpu/mesh.h>

using namespace std;

Player::Player(LogicSpace logicSpace) {
    // Position player in the middle of the logic space
    float x = logicSpace.width / 2, y = logicSpace.height / 2;

    circleCenters = { glm::vec2(x, y),
                      glm::vec2(x - 15, y - 15),
                      glm::vec2(x + 15, y - 15) };

    translateX = translateY = 0;
    
    collisionBox = {largeCircleRadius + 10, glm::vec2(circleCenters[0].x, circleCenters[0].y)};

    modelMatrix = glm::mat3(1);
}

void Player::Create() {
    Mesh* circle;
    circle = object2D::CreateCircle("player_circle1", glm::vec3(circleCenters[1], 0), smallCircleRadius, glm::vec3(1, 0.95f, 0.5f));
    meshes.push_back(circle);
    circle = object2D::CreateCircle("player_circle2", glm::vec3(circleCenters[2], 0), smallCircleRadius, glm::vec3(1, 0.95f, 0.5f));
    meshes.push_back(circle);
    circle = object2D::CreateCircle("player_circle0", glm::vec3(circleCenters[0], 0), largeCircleRadius, glm::vec3(0.1f, 0.63f, 0.88f));
    meshes.push_back(circle);
}

vector<Mesh*> const& Player::getMeshes() {
    return meshes;
}

// updates and returns the collision box
struct CircleCB* Player::getCollisionBox() {
    collisionBox.center = circleCenters[0] + glm::vec2(translateX, translateY);
    return &collisionBox;
}