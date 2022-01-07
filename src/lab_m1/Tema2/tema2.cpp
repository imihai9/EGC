#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2()
{}


Tema2::~Tema2()
{}

void Tema2::Init()
{
    window->DisablePointer(); //TODO: do this also on camera change
    renderCameraTarget = true;
    firstPersonCamera = false;

    camera = new camera::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    FOV_angle = 60.f;
    ortho_x = 30.f;
    ortho_y = 20.f;

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 1, 4));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 2, 1));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    // Render the camera target.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
        
    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->TranslateRight(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->TranslateRight(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }
    // Switch projections
    if (key == GLFW_KEY_O) {
        projectionMatrix = glm::ortho(-ortho_x, ortho_x, -ortho_y, ortho_y, 0.01f, 200.f);
    }
    else if (key == GLFW_KEY_P) {
        projectionMatrix = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);
    }

    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ||
        key == GLFW_MOUSE_BUTTON_RIGHT) {
        firstPersonCamera = !firstPersonCamera;
        // camera move forward
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;
       
    if (firstPersonCamera) {
        // Rotate the camera in first-person mode around
        // OX and OY.

        //TODO: camera move forward, dont disable target(player)
        window->DisablePointer();
        renderCameraTarget = false;
        camera->RotateFirstPerson_OX(-deltaY * sensivityOY); // looks up-down
        camera->RotateFirstPerson_OY(-deltaX * sensivityOX); // looks left-right
    }

    else {
        renderCameraTarget = true;
        window->DisablePointer();
        // Rotate the camera in third-person mode around
        // OX and OY.
        camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{}

void Tema2::OnWindowResize(int width, int height)
{}
