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
    timeRemaining = 100.f;

    window->DisablePointer();
    renderPlayer = true;
    firstPersonCamera = false;

    camera = new tema2::Camera();
    camera->Set(glm::vec3(0, 4.f, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    hudCamera = new tema2::Camera();
    hudCamera->position = glm::vec3(0);

    FOV_angle = 60.f;
    ortho_x = 30.f;
    ortho_y = 20.f;

    projectionMatrix = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);// todo delete
    projectionMatrixOrtho = glm::ortho(-ortho_x, ortho_x, -ortho_y, ortho_y, 0.01f, 200.f);
    projectionMatrixPersp = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);

    { // Create general shader
        Shader* shader = new Shader("NewShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    { // Create shader for projectle
        Shader* shader = new Shader("ProjectileShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "ProjectileVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    { // Create shader for enemy (uses vertex displacement)
        Shader* shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "EnemyVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    CreateCube("green_cube", glm::vec3(0.28f, 0.46f, 0.45f));
    CreateCube("yellow_cube", glm::vec3(0.87f, 0.8f, 0.7f));
    CreateCube("blue_cube", glm::vec3(0.22f, 0.25f, 0.58f));
    CreateCube("dark_blue_cube", glm::vec3(0.1f, 0.1f, 0.58f));
    CreateCube("red_cube", glm::vec3(0.54f, 0.f, 0.f));
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    player = new Player();
    player->translation = camera->GetTargetPositionReverse();

    maze = new Maze();
    InitMaze();
}


void Tema2::InitMaze() {
    vector<vector<int>> mazeMat = maze->getMatrix();

    for (int i = 0; i < mazeMat.size(); i++) {
        for (int j = 0; j < mazeMat[0].size(); j++) {
            if (mazeMat[i][j] == 1) { // Wall
                Wall* wall = new Wall(glm::vec3(2 * i, 1, 2 * j));
                walls.push_back(wall);
            }
            else if (mazeMat[i][j] == 2) { // Enemy
                Enemy* enemy = new Enemy(glm::vec3(2 * i, 1, 2 * j));
                enemies.push_back(enemy);
            }
            else if (mazeMat[i][j] == 3) { // Initial player pos
                player->deltaTranslation = glm::vec3(2 * i, 0, 2 * j);
                UpdatePlayer();
            }
            else if (mazeMat[i][j] == 4) {
                Wall* wall = new Wall(glm::vec3(2 * i, 1, 2 * j));
                exits.push_back(wall);
            }
        }
    }
}


// Lab 6
Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices) {
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

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    const glm::mat4& projMatrix, float elapsedTime) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int modelLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glm::mat4 viewMatrix;

    if (projMatrix == projectionMatrixPersp)
        viewMatrix = camera->GetViewMatrix();
    else if (projMatrix == projectionMatrixOrtho)
        viewMatrix = hudCamera->GetViewMatrix();

    int viewLocation = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    int timeLocation = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeLocation, elapsedTime);

    int projectionLocation = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projMatrix[0][0]);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderEntity(Entity* entity, const char* shader, float time) {
    vector<Entity::Primitive> primitives = entity->getPrimitives();

    for (Entity::Primitive primitive : primitives) {
        RenderSimpleMesh(meshes[primitive.name], shaders[shader],
            entity->modelMatrix * primitive.modelMatrix, projectionMatrixPersp, time);
    }
}

void Tema2::UpdateCameraPos() {
    float cameraDistance = 0;
    if (!firstPersonCamera) {
        cameraDistance = camera->distanceToTarget;
    }

    glm::vec3 cameraPosVec = player->translation - camera->forward * cameraDistance;
    camera->position = glm::vec3(cameraPosVec.x, camera->position.y, cameraPosVec.z);
}

void Tema2::ChangePerspective() {
    window->DisablePointer();

    firstPersonCamera = !firstPersonCamera;
    if (firstPersonCamera) {
        FOV_angle = 90.f;
        camera->TranslateForward(camera->distanceToTarget);
        camera->TranslateUpward(-0.9f);
        renderPlayer = false;
    }
    else {
        FOV_angle = 60.f;
        camera->TranslateForward(-camera->distanceToTarget);
        camera->TranslateUpward(0.9f);

        renderPlayer = true;
    }

    // Update projeciton matrix with new FOV
    projectionMatrixPersp = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);
}



bool Tema2::DetectCollision_PlayerWalls() {
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        bool collided = CheckCollisionAABB((*it)->getCollisionBox(), player->getCollisionBox());
        if (collided)
            return true;
    }

    return false;
}

bool Tema2::DetectCollision_EnemyWalls(Enemy* enemy) {
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        bool collided = CheckCollisionAABB((*it)->getCollisionBox(), enemy->getCollisionBox());
        if (collided)
            return true;
    }

    return false;
}

bool Tema2::DetectCollision_ProjWalls(Projectile* proj) {
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        bool collided = CheckCollisionSphereAABB(proj->getCollisionBox(), (*it)->getCollisionBox());
        if (collided)
            return true;
    }

    return false;
}

void Tema2::HandleCollisions_ProjectiesEnemies() {
    vector<Projectile*>::iterator it_p;
    vector<Enemy*>::iterator it_e;

    for (it_p = projectiles.begin(); it_p < projectiles.end(); ) { // for each projectile
        bool collided = false;
        for (it_e = enemies.begin(); it_e < enemies.end(); ) { // for each enemy
            collided = CheckCollisionSphereAABB((*it_p)->getCollisionBox(), (*it_e)->getCollisionBox());
            if (collided) {
                (*it_e)->hitByProj = true;
                it_e++;
                it_p = projectiles.erase(it_p);
                break;
            }
            else {
                it_e++;
            }
        }

        if (!collided)
            it_p++;
    }
}

void GameOver() {
    for (int i = 0; i < 10; i++)
        cout << '\n';
    for (int i = 0; i < 5; i++)
        cout << "Game over!\n";
    for (int i = 0; i < 10; i++)
        cout << '\n';
    exit(0);
}

void GameWon() {
    for (int i = 0; i < 10; i++)
        cout << '\n';
    for (int i = 0; i < 5; i++)
        cout << "YOU WON!\n";
    for (int i = 0; i < 10; i++)
        cout << '\n';
    exit(0);
}

void Tema2::HandleCollision_PlayerExits() {
    vector<Wall*>::iterator it;
    for (it = exits.begin(); it < exits.end(); it++) {
        bool collided = CheckCollisionAABB(player->getCollisionBox(), (*it)->getCollisionBox());
        if (collided) {
            GameWon();
        }
    }
}
void Tema2::HandleCollision_PlayerEnemies() {
    vector<Enemy*>::iterator it;
    for (it = enemies.begin(); it < enemies.end(); it++) { // for each enemy
        bool collided = CheckCollisionAABB(player->getCollisionBox(), (*it)->getCollisionBox());
        if (collided) {
            player->health -= (*it)->dmgCaused;
            if (player->health <= 0.01f) {
                GameOver();
            }
            break;
        }
    }
}

void Tema2::UpdatePlayer() {
    glm::vec3 oldTranslation = player->translation; // translation before current movement   
    player->translation = player->deltaTranslation;

    if (DetectCollision_PlayerWalls() == true)
        player->translation = oldTranslation;
    else
        UpdateCameraPos();

    player->modelMatrix = glm::translate(glm::mat4(1), player->translation);
    player->modelMatrix = glm::rotate(player->modelMatrix, playerRotateAngle, glm::vec3(0, 1, 0));

    if (renderPlayer)
        RenderEntity(player, "NewShader", 0);

    ////// Collision box render test
 /*    glm::mat4 cbox_modelMatrix = glm::mat4(1);
     cbox_modelMatrix = glm::translate(cbox_modelMatrix, player->translation);
     cbox_modelMatrix = glm::translate(cbox_modelMatrix, glm::vec3(0, 0.725f, 0));
     cbox_modelMatrix = glm::scale(cbox_modelMatrix, glm::vec3(0.6f / 2.f, 1.45f / 2.f, 0.6f / 2.f));
     RenderSimpleMesh(meshes["yellow_cube"], shaders["NewShader"], cbox_modelMatrix, projectionMatrixPersp, 0);*/
}

void Tema2::UpdateWalls() {
    vector<Wall*>::iterator it;
    for (it = walls.begin(); it < walls.end(); it++) {
        RenderEntity(*it, "NewShader", 0);
    }
}

void Tema2::UpdateEnemies(float deltaTimeSeconds) {
    vector<Enemy*>::iterator it;
    for (it = enemies.begin(); it < enemies.end(); ) {
        float dist = deltaTimeSeconds * (*it)->speed;

        (*it)->translation += (*it)->directions[(*it)->currDir] * dist;
        (*it)->modelMatrix = glm::translate(glm::mat4(1), (*it)->translation);

        // if (DetectCollision_EnemyWalls(*it)) { // version with movement restricted by walls
        // movement restricted by current cell
        bool reachedWall = false;

        float x_size_halved = (*it)->size.x / 2;
        float z_size_halved = (*it)->size.z / 2;

        switch ((*it)->currDir) {
        case 0: //OX +
            if ((*it)->translation.x >= 1 - x_size_halved)
                reachedWall = true;
        case 1: //OZ +
            if ((*it)->translation.z >= 1 - z_size_halved)
                reachedWall = true;
        case 2: //OX -
            if ((*it)->translation.x <= -1 + x_size_halved)
                reachedWall = true;
        default: // case 3: OZ -
            if ((*it)->translation.z <= -1 + z_size_halved)
                reachedWall = true;
        }

        if (reachedWall) {
            // Revert translation that led to collision
            (*it)->translation -= (*it)->directions[(*it)->currDir] * dist;
            (*it)->ChangeDir();
        }

        RenderEntity(*it, "EnemyShader", (*it)->timeSinceHit);

        if ((*it)->hitByProj) {
            if ((*it)->timeSinceHit >= (*it)->timeAliveAfterHit)
                it = enemies.erase(it);
            else {
                (*it)->timeSinceHit += deltaTimeSeconds;
                it++;
            }
        }
        else {
            it++;
        }
    }
}

void Tema2::UpdateCrosshair() {
    if (firstPersonCamera) {
        // Render crosshair in hud camera
        glm::mat4 crosshairModelMat = glm::scale(glm::mat4(1), glm::vec3(0.3));
        RenderSimpleMesh(meshes["green_cube"], shaders["NewShader"], crosshairModelMat, projectionMatrixOrtho, 0);
    }
}

void Tema2::UpdateBar(float metric, float height) {
    glm::mat4 maxHealthModelMatrix = glm::mat4(1);
    maxHealthModelMatrix = glm::translate(maxHealthModelMatrix, glm::vec3(20, height, 0));
    maxHealthModelMatrix = glm::scale(maxHealthModelMatrix, glm::vec3(4, 1, 1));
    // Translate so that the left corner would have its X,Y coordinates in the origin
    maxHealthModelMatrix = glm::translate(maxHealthModelMatrix, glm::vec3(1, 1, 0));

    glm::mat4 currHealthModelMatrix = glm::mat4(1);
    currHealthModelMatrix = glm::translate(currHealthModelMatrix, glm::vec3(20, height, 0));
    currHealthModelMatrix = glm::scale(currHealthModelMatrix, glm::vec3((float)(metric / 100), 1, 1));
    currHealthModelMatrix = glm::scale(currHealthModelMatrix, glm::vec3(4, 1, 1));
    currHealthModelMatrix = glm::translate(currHealthModelMatrix, glm::vec3(1, 1, 0));

    RenderSimpleMesh(meshes["green_cube"], shaders["NewShader"], currHealthModelMatrix, projectionMatrixOrtho, 0);
    RenderSimpleMesh(meshes["red_cube"], shaders["NewShader"], maxHealthModelMatrix, projectionMatrixOrtho, 0);
}

void Tema2::SpawnProjectile() {
    glm::vec3 projInitialPos = player->translation + glm::vec3(0, 1, 0);

    glm::vec3 camProjDiff = -glm::vec3(0, camera->position.y, 0) + glm::vec3(0, projInitialPos.y, 0);
    glm::vec3 projVector = camera->forward * 3.f - camProjDiff;

    glm::vec3 normalizedProjVector = glm::normalize(projVector);
    Projectile* proj = new Projectile(projInitialPos, normalizedProjVector);
    projectiles.push_back(proj);
}

void Tema2::UpdateProjectiles(float deltaTimeSeconds) {
    vector<Projectile*>::iterator it;
    for (it = projectiles.begin(); it < projectiles.end(); ) {
        // Reduce time
        (*it)->remainingTime -= deltaTimeSeconds;
        if ((*it)->remainingTime <= 0 || DetectCollision_ProjWalls(*it) == true) {
            it = projectiles.erase(it);
        }
        else {
            glm::vec3 translation = (*it)->direction * (*it)->speed * deltaTimeSeconds;
            (*it)->translation += translation;
            //(*it)->modelMatrix = glm::translate((*it)->modelMatrix, translation);
            (*it)->modelMatrix = glm::translate(glm::mat4(1), (*it)->translation);
            RenderEntity(*it, "ProjectileShader", 0);
            it++;
        }
    }
}




void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    timeRemaining -= deltaTimeSeconds * 2;
    if (timeRemaining <= 0.01f) {
        GameOver();
    }

    UpdateBar(timeRemaining, 10);
    UpdateBar(player->health, 15);

    HandleCollision_PlayerExits();
    HandleCollision_PlayerEnemies();
    HandleCollisions_ProjectiesEnemies();
    UpdatePlayer();
    UpdateWalls();
    UpdateProjectiles(deltaTimeSeconds);
    UpdateEnemies(deltaTimeSeconds);
    UpdateCrosshair();

}


void Tema2::FrameEnd()
{
    // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W) ||
        window->KeyHold(GLFW_KEY_S) ||
        window->KeyHold(GLFW_KEY_A) ||
        window->KeyHold(GLFW_KEY_D)) {

        player->deltaTranslation = player->translation;

        glm::vec3 normalForward = glm::normalize(camera->forward);
        glm::vec3 normalRight = glm::normalize(camera->right);

        if (window->KeyHold(GLFW_KEY_W)) {
            player->deltaTranslation += glm::vec3(normalForward.x, 0, normalForward.z) * player->speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            player->deltaTranslation += glm::vec3(normalForward.x, 0, normalForward.z) * (-player->speed) * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            player->deltaTranslation += normalRight * (-player->speed) * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            player->deltaTranslation += normalRight * player->speed * deltaTime;
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
        SpawnProjectile();
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
        ChangePerspective();
    if (key == GLFW_KEY_X)
        window->DisablePointer();
    // Switch projections
    if (key == GLFW_KEY_O)
        projectionMatrix = glm::ortho(-ortho_x, ortho_x, -ortho_y, ortho_y, 0.01f, 200.f);
    else if (key == GLFW_KEY_P)
        projectionMatrix = glm::perspective(RADIANS(FOV_angle), window->props.aspectRatio, 0.01f, 200.0f);
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
{
    if (firstPersonCamera && button == 1) {
        SpawnProjectile();
    }

    if (button == 2) {
        ChangePerspective();
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{}

void Tema2::OnWindowResize(int width, int height)
{}
