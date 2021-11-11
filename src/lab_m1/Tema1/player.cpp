
#include "player.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"
#include <core/gpu/mesh.h>
#include <string>

using namespace std;

Player::Player() {
    meshCount = 3;
    int x = 600, y = 300;
    circleCenters = { glm::vec3(x, y, 0),
                      glm::vec3(x - 20, y - 20, 0),
                      glm::vec3(x + 20, y - 20, 0) };
    modelMatrix = glm::mat3(1);
}

void Player::Create() {
   // int squareSide = 200;
   // glm::vec3 corner = glm::vec3(-squareSide / 2, -squareSide / 2, 0);

    //Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
   // meshes.push_back(square1);


    Mesh* circle;
    circle = object2D::CreateCircle("player_circle1", circleCenters[1], 15, glm::vec3(1, 0.95f, 0.5f));
    meshes.push_back(circle);
    circle = object2D::CreateCircle("player_circle2", circleCenters[2], 15, glm::vec3(1, 0.95f, 0.5f));
    meshes.push_back(circle);
    circle = object2D::CreateCircle("player_circle0", circleCenters[0], 30, glm::vec3(0.1f, 0.63f, 0.88f));
    meshes.push_back(circle);
}

vector<Mesh*> const& Player::getMeshes() {
    return meshes;
}