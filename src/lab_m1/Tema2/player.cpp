#include "lab_m1/Tema2/player.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>

using namespace tema2;
using namespace std;

Player::Player() {
    // Position player in the middle of the logic space
    translateX = translateY = 0;
    modelMatrix = glm::mat3(1); // global model matrix
    Create();
}

void Player::Create() {

    /* Calculations
        Cube (primitive) len = 2
        Head len = 2 * 0.2 = 0.4

        -1.f (translate down on OY in order to get in center)
     */

    float ty = 0.7f; // global ty, to get player with feet on ground

    // Head
    glm::mat4 m_head = glm::translate(glm::mat4(1), glm::vec3(0, 2.f + ty, 0));
    m_head = glm::scale(m_head, glm::vec3(0.2f));

    // Torso
    glm::mat4 m_torso = glm::translate(glm::mat4(1), glm::vec3(0, 1.15f + ty, 0));
    m_torso = glm::scale(m_torso, glm::vec3(1.7f, 3.f, 1.f));
    m_torso = glm::scale(m_torso, glm::vec3(0.2f));

    // Hands
    glm::mat4 m_hand1 = glm::translate(glm::mat4(1), glm::vec3(0.6f, 0.75f + ty, 0));
    m_hand1 = glm::scale(m_hand1, glm::vec3(0.2f));

    glm::mat4 m_hand2 = glm::translate(glm::mat4(1), glm::vec3(-0.6f, 0.75f + ty, 0));
    m_hand2 = glm::scale(m_hand2, glm::vec3(0.2f));

    // Arms
    glm::mat4 m_arm1 = glm::translate(glm::mat4(1), glm::vec3(0.6f, 1.37f + ty, 0));
    m_arm1 = glm::scale(m_arm1, glm::vec3(1.f, 1.9f, 1.f));
    m_arm1 = glm::scale(m_arm1, glm::vec3(0.2f));

    glm::mat4 m_arm2 = glm::translate(glm::mat4(1), glm::vec3(-0.6f, 1.37f + ty, 0));
    m_arm2 = glm::scale(m_arm2, glm::vec3(1.f, 1.9f, 1.f));
    m_arm2 = glm::scale(m_arm2, glm::vec3(0.2f));

    // Legs
    glm::mat4 m_leg1 = glm::translate(glm::mat4(1), glm::vec3(0));
    m_leg1 = glm::translate(glm::mat4(1), glm::vec3(0.19f, -0.1f + ty, 0));
    m_leg1 = glm::scale(m_leg1, glm::vec3(0.47f, 1.f, 1.f));
    m_leg1 = glm::scale(m_leg1, glm::vec3(1.7f, 3.f, 1.f)); //torso size
    m_leg1 = glm::scale(m_leg1, glm::vec3(0.2f));

    glm::mat4 m_leg2 = glm::translate(glm::mat4(1), glm::vec3(0));
    m_leg2 = glm::translate(glm::mat4(1), glm::vec3(-0.19f, -0.1f + ty, 0));
    m_leg2 = glm::scale(m_leg2, glm::vec3(0.47f, 1.f, 1.f));
    m_leg2 = glm::scale(m_leg2, glm::vec3(1.7f, 3.f, 1.f)); //torso size
    m_leg2 = glm::scale(m_leg2, glm::vec3(0.2f));

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
/*
struct CircleCB* Player::getCollisionBox() {
   // collisionBox.center = circleCenters[0] + glm::vec2(translateX, translateY);
   // return &collisionBox;
}
*/