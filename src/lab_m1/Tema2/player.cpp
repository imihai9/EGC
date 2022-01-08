#include "lab_m1/Tema2/player.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

Player::Player() {
    translateX = translateY = 0;
    modelMatrix = glm::mat4(1); // global model matrix
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    Create();

}

void printMatrix(glm::mat4 mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << mat[i][j] << ' ';
        }
        cout << '\n';
    }

}

void printVector(glm::vec3 vec) {
    for (int i = 0; i < 3; i++) {
        cout << vec[i] << ' ';
    }
}

void Player::Create() {

    /* Calculations
        Cube (primitive) len = 2
        Head len = 2 * 0.2 = 0.4

        -1.f (translate down on OY in order to get in center)
     */

    float ty = 0.7f; // global ty, to get player with feet on ground
    float tz = 0.f;

    float scaleFactor = 0.2f;
    float scaleFactorFinal = 0.5f;
    // Head
    glm::mat4 m_head = glm::mat4(1);
    m_head = glm::scale(m_head, glm::vec3(scaleFactorFinal));
    m_head = glm::translate(m_head, glm::vec3(0, 2.f + ty, tz));
    m_head = glm::scale(m_head, glm::vec3(scaleFactor));

  //  printMatrix(m_head);
    //printVec()
    // Torso
    glm::mat4 m_torso = glm::mat4(1);
    m_torso = glm::scale(m_torso, glm::vec3(scaleFactorFinal));
    m_torso = glm::translate(m_torso, glm::vec3(0, 1.15f + ty, tz));
    m_torso = glm::scale(m_torso, glm::vec3(1.7f, 3.f, 1.f));
    m_torso = glm::scale(m_torso, glm::vec3(scaleFactor));

    // Hands
    glm::mat4 m_hand1 = glm::mat4(1);
    m_hand1 = glm::scale(m_hand1, glm::vec3(scaleFactorFinal));
    m_hand1 = glm::translate(m_hand1, glm::vec3(0.6f, 0.75f + ty, tz));
    m_hand1 = glm::scale(m_hand1, glm::vec3(scaleFactor));

    glm::mat4 m_hand2 = glm::mat4(1);
    m_hand2 = glm::scale(m_hand2, glm::vec3(scaleFactorFinal));
    m_hand2 = glm::translate(m_hand2, glm::vec3(-0.6f, 0.75f + ty, tz));
    m_hand2 = glm::scale(m_hand2, glm::vec3(scaleFactor));

    // Arms
    glm::mat4 m_arm1 = glm::mat4(1);
    m_arm1 = glm::scale(m_arm1, glm::vec3(scaleFactorFinal));
    m_arm1 = glm::translate(m_arm1, glm::vec3(0.6f, 1.37f + ty, tz));
    m_arm1 = glm::scale(m_arm1, glm::vec3(1.f, 1.9f, 1.f));
    m_arm1 = glm::scale(m_arm1, glm::vec3(scaleFactor));

    glm::mat4 m_arm2 = glm::mat4(1);
    m_arm2 = glm::scale(m_arm2, glm::vec3(scaleFactorFinal));
    m_arm2 = glm::translate(m_arm2, glm::vec3(-0.6f, 1.37f + ty, tz));
    m_arm2 = glm::scale(m_arm2, glm::vec3(1.f, 1.9f, 1.f));
    m_arm2 = glm::scale(m_arm2, glm::vec3(scaleFactor));

    // Legs
    glm::mat4 m_leg1 = glm::mat4(1);
    m_leg1 = glm::scale(m_leg1, glm::vec3(scaleFactorFinal));
    m_leg1 = glm::translate(m_leg1, glm::vec3(0.19f, -0.1f + ty, tz));
    m_leg1 = glm::scale(m_leg1, glm::vec3(0.47f, 1.f, 1.f));
    m_leg1 = glm::scale(m_leg1, glm::vec3(1.7f, 3.f, 1.f)); //torso size
    m_leg1 = glm::scale(m_leg1, glm::vec3(scaleFactor));

    glm::mat4 m_leg2 = glm::mat4(1);
    m_leg2 = glm::scale(m_leg2, glm::vec3(scaleFactorFinal));
    m_leg2 = glm::translate(m_leg2, glm::vec3(-0.19f, -0.1f + ty, tz));
    m_leg2 = glm::scale(m_leg2, glm::vec3(0.47f, 1.f, 1.f));
    m_leg2 = glm::scale(m_leg2, glm::vec3(1.7f, 3.f, 1.f)); //torso size
    m_leg2 = glm::scale(m_leg2, glm::vec3(scaleFactor));

    primitives.push_back({ "yellow_cube" , m_head });
    primitives.push_back({ "green_cube" , m_torso });
    primitives.push_back({ "green_cube" , m_arm1 });
    primitives.push_back({ "green_cube" , m_arm2 });
    primitives.push_back({ "yellow_cube" , m_hand1 });
    primitives.push_back({ "yellow_cube" , m_hand2 });
    primitives.push_back({ "blue_cube" , m_leg1 });
    primitives.push_back({ "blue_cube" , m_leg2 });
}

std::vector<Entity::Primitive> const& Player::getPrimitives() {
    return primitives;
}

vector<glm::vec3> canonicalCube = {
    glm::vec3(-1, -1,  1),
    glm::vec3(1, -1,  1),
    glm::vec3(-1,  1,  1),
    glm::vec3(1,  1,  1),
    glm::vec3(-1, -1, -1),
    glm::vec3(1, -1, -1),
    glm::vec3(-1,  1, -1),
    glm::vec3(1,  1, -1)
};

// updates and returns the collision box
AABB* Player::getCollisionBox() {
    // Collision box render test
    glm::mat4 cbox_modelMatrix = this->modelMatrix;
    cbox_modelMatrix = glm::translate(cbox_modelMatrix, glm::vec3(0, 0.725f, 0));
    cbox_modelMatrix = glm::scale(cbox_modelMatrix, glm::vec3(0.8f / 2.f, 1.45f / 2.f, 0.2f / 2.f));

    glm::vec3 cbox_vertex = cbox_modelMatrix * glm::vec4(canonicalCube[0], 1);
    collisionBox.minX = collisionBox.maxX = cbox_vertex.x;
    collisionBox.minY = collisionBox.maxY = cbox_vertex.y;
    collisionBox.minZ = collisionBox.maxZ = cbox_vertex.z;

    for (int i = 1; i < canonicalCube.size(); i++) {
        glm::vec3 cbox_vertex = cbox_modelMatrix * glm::vec4(canonicalCube[i], 1);
        collisionBox.minX = min(collisionBox.minX, cbox_vertex.x);
        collisionBox.minY = min(collisionBox.minY, cbox_vertex.y);
        collisionBox.minZ = min(collisionBox.minZ, cbox_vertex.z);
        collisionBox.maxX = max(collisionBox.maxX, cbox_vertex.x);
        collisionBox.maxY = max(collisionBox.maxY, cbox_vertex.y);
        collisionBox.maxZ = max(collisionBox.maxZ, cbox_vertex.z);
    }

    //cout << "p x: " << collisionBox.minX << ' ' << collisionBox.maxX << endl;
   // cout << "p y: " << collisionBox.minY << ' ' << collisionBox.maxY << endl;
   // cout << "p z: " << collisionBox.minZ << ' ' << collisionBox.maxZ << endl;
    return &collisionBox;
}
