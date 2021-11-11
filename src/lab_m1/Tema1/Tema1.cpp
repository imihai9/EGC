#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

#include "lab_m1/lab3/lab3_vis2D.h"
/* TODOs:
- Somehow, place the init and render methods in PLAYER.H. / ..
- Make player, enemy, ... classes inherit / override CreateMesh

*/


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::InitEntity(Entity *entity) {
    entity->Create();

    vector<Mesh*> entityMeshes = entity->getMeshes();
    for (Mesh *m : entityMeshes) { // Indexing entity meshes
        AddMeshToList(m);
    }
}

void Tema1::RenderEntity(Entity* entity) {
    vector<Mesh*> entityMeshes = entity->getMeshes();
    for (Mesh* m : entityMeshes) {
        RenderMesh2D(m, shaders["VertexColor"], entity->modelMatrix);
    }
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    /*Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);*/

    translateX = 0;
    translateY = 0;

    rotationAngle = 0;
    
    mouseY = 0;
    mouseX = 1;

    player = new Player();
    InitEntity(player);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    rotationAngle = atan2(mouseX - 600 - translateX, mouseY - 300 + translateY);

    player->modelMatrix = transform2D::Translate(translateX, translateY) *
        transform2D::Translate(600, 300) *   // Initial object center is currently (600, 300)
        transform2D::Rotate(rotationAngle) *
        transform2D::Translate(-600, -300);
    RenderEntity(player);
}

void Tema1::FrameEnd()
{
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    const float translateSpeed = 200.f;
    if (window->KeyHold(GLFW_KEY_S))
        translateY -= deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_W))
        translateY += deltaTime * translateSpeed;

    if (window->KeyHold(GLFW_KEY_A))
        translateX -= deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_D))
        translateX += deltaTime * translateSpeed;
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    this->mouseX = mouseX;
    this->mouseY = mouseY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
