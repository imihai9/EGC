#include "lab_m1/Tema2/player.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>
#include <iostream>

using namespace tema2;
using namespace std;

Player::Player() {
    health = 50;
    translation = glm::vec3(0);
    deltaTranslation = glm::vec3(0);
    modelMatrix = glm::mat4(1); // global model matrix
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    Create();
    InitCollisionBox();
}

void Player::InitCollisionBox() {
    // scale: 0.6f, 1.45f, 0.6f
    // initial translate: 0, 0.725f, 0
    /* => initial sizes: 
        minX= -0.3f
        maxX= 0.3f
        minY= 0         (on ground)
        maxY= 1.45f     (height)
        minZ= -0.3f
        maxZ= 0.3f
    */

    collisionBox.min = glm::vec3(-0.3f, 0, -0.3f);
    collisionBox.max = glm::vec3(0.3f, 1.45f, 0.3f);
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
// updates and returns the collision box
AABB* Player::getCollisionBox() {
    
    updatedCollisionBox.min = collisionBox.min + this->translation;
    updatedCollisionBox.max = collisionBox.max + this->translation;

    //cout << "p x: " << collisionBox.minX << ' ' << collisionBox.maxX << endl;
   // cout << "p y: " << collisionBox.minY << ' ' << collisionBox.maxY << endl;
   // cout << "p z: " << collisionBox.minZ << ' ' << collisionBox.maxZ << endl;
    return &updatedCollisionBox;
}
