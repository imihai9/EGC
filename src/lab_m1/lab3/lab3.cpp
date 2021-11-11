#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}

float squareSide = 100;

void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angle = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    int razaOrbitaPamant = 200;
    int razaOrbitaLuna = 50;

    translateX += deltaTimeSeconds * 10.f;
    translateY += deltaTimeSeconds * 5.f;

    glm::mat3 modelMatrixSoare = transform2D::Translate(250 + translateX, 250 + translateY) *
                                 transform2D::Rotate(angle);
                                
    { // Soarele
        modelMatrix =
            modelMatrixSoare *
            transform2D::Translate(-squareSide / 2, -squareSide / 2);
                      
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }

    glm::mat3 modelMatrixPamant = modelMatrixSoare *
        transform2D::Rotate(2 * angle) *
        transform2D::Translate(razaOrbitaPamant, 0) *
        transform2D::Translate(squareSide / 2, squareSide / 2) *
        transform2D::Rotate(angle) *
        transform2D::Scale(0.5f, 0.5f);

    { // Pamantul
        modelMatrix = modelMatrixPamant *
            transform2D::Translate(-squareSide / 2, -squareSide / 2);

        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }

    { // Luna
        // scaleX = sin(Engine::GetElapsedTime());
        // scaleY = cos(Engine::GetElapsedTime() * 2.f);
        modelMatrix = modelMatrixPamant *
            transform2D::Rotate(2 * angle) *
            transform2D::Translate(razaOrbitaLuna, 0) *
            transform2D::Translate(squareSide/2, squareSide/2) *
            transform2D::Rotate(angle) *
            transform2D::Scale(0.2f, 0.2f) *
            transform2D::Translate(-squareSide / 2, -squareSide / 2);
        RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    }

    angle += deltaTimeSeconds * 0.6f;
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
