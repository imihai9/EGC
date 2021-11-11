#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Initialize extra vars
    t_cube = 0;
    tx_cube = 1, ty_cube = 1;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::Update(float deltaTimeSeconds)
{
    t_cube += deltaTimeSeconds * 1.5f;
    tx_cube = t_cube;
    ty_cube = abs(sin(2*t_cube));

    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // Bonus1 - Jump and Flip
    // Cubul principal
    glm::mat4 modelMatrixCube = glm::mat4(1);
    modelMatrixCube *= transform3D::Translate(0.0f, 0.0f, 0.0f);
    modelMatrixCube *= transform3D::Translate(tx_cube, ty_cube, 0);
    modelMatrixCube *= transform3D::RotateOZ(-2*t_cube);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrixCube);

    // Sageata 1
    glm::mat4 modelMatrixArrow = glm::mat4(1);
    modelMatrixArrow *= transform3D::Translate(-0.18f, 0.18f, 0);
    modelMatrixArrow *= transform3D::RotateOZ(M_PI / 4);
    modelMatrixArrow *= transform3D::Scale(0.1f, 0.6f, 1.05f);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrixCube * modelMatrixArrow);

    // Sageata 2
    glm::mat4 modelMatrixArrow2 = transform3D::RotateOZ(- M_PI / 2) * modelMatrixArrow;
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrixCube * modelMatrixArrow2);
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    const float translateSpeed = 2.f;
    if (window->KeyHold(GLFW_KEY_W))
        translateZ -= deltaTime * translateSpeed;  // axa Z
    else if (window->KeyHold(GLFW_KEY_S))
        translateZ += deltaTime * translateSpeed;

    if (window->KeyHold(GLFW_KEY_A))               // axa X
        translateX -= deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_D))
        translateX += deltaTime * translateSpeed;

    if (window->KeyHold(GLFW_KEY_R))               // axa Y
        translateY += deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_F))
        translateY -= deltaTime * translateSpeed;
 
    if (window->KeyHold(GLFW_KEY_1))
        scaleX = scaleY = scaleZ = scaleX + deltaTime; // scalare uniforma

    else if (window->KeyHold(GLFW_KEY_2))
        scaleX = scaleY = scaleZ = scaleY - deltaTime;

    if (window->KeyHold(GLFW_KEY_3))
        angularStepOX += deltaTime;
    else if (window->KeyHold(GLFW_KEY_4))
        angularStepOX -= deltaTime;

    if (window->KeyHold(GLFW_KEY_5))
        angularStepOY -= deltaTime;
    else if (window->KeyHold(GLFW_KEY_6))
        angularStepOY += deltaTime;

    if (window->KeyHold(GLFW_KEY_7))
        angularStepOZ += deltaTime;
    else if (window->KeyHold(GLFW_KEY_8))
        angularStepOZ -= deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
