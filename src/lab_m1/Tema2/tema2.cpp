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

void Tema2::InitMaze() {
    vector<vector<int>> mazeMat = maze->getMatrix();

    for (int i = 0; i < mazeMat.size(); i++) {
        for (int j = 0; j < mazeMat[0].size(); j++) {
            if (mazeMat[i][j] == 1) {
                // Wall
                Wall* wall = new Wall(glm::vec3(2 * i, 1, 2 * j));
                walls.push_back(wall);
            }
            // 0, 2..
        }
    }
}

void Tema2::Init()
{
    window->DisablePointer(); //TODO: do this also on camera change
    renderPlayer = true;
    firstPersonCamera = false;

    camera = new tema2::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    FOV_angle = 60.f;
    ortho_x = 30.f;
    ortho_y = 20.f;

    projectionMatrix = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);

    // Create a shader program
    {
        Shader* shader = new Shader("NewShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    CreateCube("green_cube", glm::vec3(0.28f, 0.46f, 0.45f));
    CreateCube("yellow_cube", glm::vec3(0.87f, 0.8f, 0.7f));
    CreateCube("blue_cube", glm::vec3(0.22f, 0.25f, 0.58f));

    player = new Player();
    player->translation = camera->GetTargetPositionReverse();

    maze = new Maze();
    InitMaze();
}

// Lab 6
Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::CreateCube(const char* name, glm::vec3 color) {
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), color, glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, -1,  1),  color, glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), color, glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(1,  1,  1),  color, glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), color, glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(1, -1, -1),  color, glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), color, glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(1,  1, -1),  color, glm::vec3(0.1, 0.5, 0.8)),
        };
        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh(name, vertices, indices);
    }
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int viewLocation = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    int timeLocation = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    int projectionLocation = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderEntity(Entity* entity) {
    vector<Entity::Primitive> primitives = entity->getPrimitives();

    for (Entity::Primitive primitive : primitives) {
        RenderSimpleMesh(meshes[primitive.name], shaders["NewShader"], entity->modelMatrix * primitive.modelMatrix);
    }
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

bool Tema2::HandleCollisions() {
    // {Player - Walls}
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        bool collided = CheckCollisionAABB((*it)->getCollisionBox(), player->getCollisionBox());
        if (collided)
            return true;
        /*
        if (collided) {
           cout << "c";
        }*/
    }

    return false;
}

glm::vec3 tmpPlayerTranslate;

void Tema2::UpdatePlayer() {
    /*
    if (firstPersonCamera)
        playerPos = camera->GetTargetPositionFPS();
    else
        playerPos = camera->GetTargetPosition();
    */

    glm::vec3 oldTranslation = player->translation; // translation before current movement   
    player->translation = tmpPlayerTranslate;

    if (HandleCollisions() == true) { // TODO: only if player has moved
        player->translation = oldTranslation;
    }
    else {
        glm::vec3 cameraPosVec = player->translation - camera->forward * camera->distanceToTarget;
        camera->position = glm::vec3 (cameraPosVec.x, camera->position.y, cameraPosVec.z);

        //glm::vec3 distance = player->translation - oldTranslation;
        //  if (distance.x != 0)
        //      camera->TranslateRight(distance.x);
        //  if (distance.z != 0)
        //      camera->MoveForward(distance.z);
    }

    player->modelMatrix = glm::translate(glm::mat4(1), player->translation);
    player->modelMatrix = glm::rotate(player->modelMatrix, playerRotateAngle, glm::vec3(0, 1, 0));

    if (renderPlayer)
        RenderEntity(player);

    //// Collision box render test
    glm::mat4 cbox_modelMatrix = glm::mat4(1);
    cbox_modelMatrix = glm::translate(cbox_modelMatrix, player->translation);
    cbox_modelMatrix = glm::translate(cbox_modelMatrix, glm::vec3(0, 0.725f, 0));
    cbox_modelMatrix = glm::scale(cbox_modelMatrix, glm::vec3(0.6f / 2.f, 1.45f / 2.f, 0.6f / 2.f));
    RenderSimpleMesh(meshes["yellow_cube"], shaders["NewShader"], cbox_modelMatrix);

}

void Tema2::UpdateWalls() {
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        RenderEntity(*it);
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    UpdatePlayer();
    UpdateWalls();
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    float playerSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W) ||
        window->KeyHold(GLFW_KEY_S) ||
        window->KeyHold(GLFW_KEY_A) ||
        window->KeyHold(GLFW_KEY_D)) {

        tmpPlayerTranslate = player->translation;


        if (window->KeyHold(GLFW_KEY_W)) {
            tmpPlayerTranslate += glm::vec3(camera->forward.x, 0, camera->forward.z) * playerSpeed * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            tmpPlayerTranslate += glm::vec3(camera->forward.x, 0, camera->forward.z) * (-playerSpeed) * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            tmpPlayerTranslate += camera->right * (-playerSpeed) * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            tmpPlayerTranslate += camera->right * playerSpeed * deltaTime;
        }
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
    if (key == GLFW_KEY_C) {
        HandleCollisions();
    }

    if (key == GLFW_KEY_X) {
        window->DisablePointer();
    }

    if (key == GLFW_KEY_T) {
        renderPlayer = !renderPlayer;
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

        window->DisablePointer();

        firstPersonCamera = !firstPersonCamera;
        if (firstPersonCamera) {
            camera->TranslateForward(camera->distanceToTarget);
            renderPlayer = false;
        }
        else {
            camera->TranslateForward(-camera->distanceToTarget);
            renderPlayer = true;
        }
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    playerRotateAngle += -deltaX * sensivityOX;

    if (firstPersonCamera) {
        // Rotate the camera in first-person mode around
        // OX and OY.

        camera->RotateFirstPerson_OX(-deltaY * sensivityOY); // looks up-down
        camera->RotateFirstPerson_OY(-deltaX * sensivityOX); // looks left-right
    }

    else {

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
