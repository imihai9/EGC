#include "lab_m1/Tema1/Tema1.h"
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

/* TODOs:
- Somehow, place the init and render methods in PLAYER.H. / ..
- Make player, enemy, ... classes inherit / override CreateMesh

*/

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::InitEntity(Entity* entity) {
    entity->Create();

    vector<Mesh*> entityMeshes = entity->getMeshes();
    for (Mesh* m : entityMeshes) { // Indexing entity meshes
        AddMeshToList(m);
    }
}

void Tema1::RenderEntity(Entity* entity) {
    vector<Mesh*> entityMeshes = entity->getMeshes();
    for (Mesh* m : entityMeshes) {
        RenderMesh2D(m, shaders["VertexColor"], entity->modelMatrix);
    }
}

void Tema1::GenerateObstacle(glm::vec3 leftCorner, glm::vec2 obstSize, glm::vec3 obstColor) {
    Obstacle* obst = new Obstacle(logicSpace, leftCorner, obstSize, obstColor);
    InitEntity(obst);
    obstacles.push_back(obst);
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    //glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    
    glEnable(GL_SCISSOR_TEST);
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    float left = (float)viewSpace.x;
    float right = (float)(viewSpace.x + viewSpace.width);
    float bottom = (float)viewSpace.y;
    float top = (float)(viewSpace.y + viewSpace.height);
    
    float tx = (viewSpace.width - viewSpace.width * resize_factor) / 2;
    float ty = (viewSpace.height - viewSpace.height * resize_factor) / 2;

    if (overview_toggle == 1) 
        GetSceneCamera()->SetOrthographic(left, right, bottom, top, 0.1f, 400);
    else
        GetSceneCamera()->SetOrthographic(left + tx, right - tx, bottom + ty, top - ty, 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1280;   // logic width
    logicSpace.height = 720;  // logic height

    rotationAngle = 0;

    mouseY = 0;
    mouseX = 1;

    resize_factor = 0.75;
    overview_toggle = 0;

    // Player instantiation
    player = new Player(logicSpace);
    InitEntity(player);

    // Map instantiation
    map = new Map(logicSpace);
    InitEntity(map);

    // Obstacle objects instantiation
    glm::vec3 color = glm::vec3(0.2f, 0.5f, 1.f);
    GenerateObstacle(glm::vec3(logicSpace.x + 200, logicSpace.y + 300, 0), glm::vec2(150, 200), color);
    GenerateObstacle(glm::vec3(logicSpace.x + 400, logicSpace.y + 150, 0), glm::vec2(300, 50), color);
    GenerateObstacle(glm::vec3(logicSpace.x + 1000, logicSpace.y + 300, 0), glm::vec2(50, 300), color);
    GenerateObstacle(glm::vec3(logicSpace.x + 500, logicSpace.y + 500, 0), glm::vec2(100, 100), color);
    GenerateObstacle(glm::vec3(logicSpace.x + 800, logicSpace.y + 250, 0), glm::vec2(50, 300), color);

    // Projectile instantiation
    projectile = new Projectile(logicSpace);
    InitEntity(projectile);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Collision detection
    { // {Player - Map walls}
        glm::vec2 penetration = CheckCollisionRectCircleInside(map->getCollisionBox(), player->getCollisionBox());
        if (penetration != glm::vec2(0)) {
            player->translateX += penetration.x;
            player->translateY += penetration.y;
        }
    }
    
    { // {Player - Obstacles}
        for (Obstacle* obst : obstacles) {
            glm::vec2 penetration = CheckCollisionRectCircle(obst->getCollisionBox(), player->getCollisionBox());
            if (penetration != glm::vec2(0)) {
                player->translateX += penetration.x;
                player->translateY += penetration.y;
            }
        }
    }

    { // {Projectile - Map}
        bool penOccured = false;
        vector<Projectile::ProjectileData>::iterator it;
        for (it = projData.begin(); it < projData.end(); ) {
            // {Projectile - Map walls}
            glm::vec2 penetration = CheckCollisionRectCircleInside(map->getCollisionBox(), &(*it).collisionBox);
            if (penetration != glm::vec2(0))
                it = projData.erase(it);
            else { // {Projectile - Obstacles}
                for (Obstacle* obst : obstacles) {
                    glm::vec2 penetration = CheckCollisionRectCircle(obst->getCollisionBox(), &(*it).collisionBox);
                    if (penetration != glm::vec2(0)) {
                        penOccured = true;
                        break;
                    }
                }

                if (penOccured) {
                    it = projData.erase(it);
                    penOccured = false;
                }
                else
                    it++;
            }
        }
    }


    // Camera setup
    auto camera = GetSceneCamera();

    // transform translation factors from logical space to viewport space (in order to keep player centered
    float tx_view = (float)(viewSpace.x + (viewSpace.width - viewSpace.x) * (player->translateX - logicSpace.x) / (logicSpace.width - logicSpace.x));
    float ty_view = (float)(viewSpace.y + (viewSpace.height - viewSpace.y) * (player->translateY - logicSpace.y) / (logicSpace.height - logicSpace.y));

    camera->SetPosition(glm::vec3(tx_view, ty_view, 50));
    camera->Update();
    GetCameraInput()->SetActive(false);
}

void Tema1::Update(float deltaTimeSeconds)
{
      glm::ivec2 resolution = window->GetResolution();

      // Sets the screen area where to draw
      viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
      SetViewportArea(viewSpace, glm::vec3(0), true);

      // Compute the 2D visualization matrix
      visMatrix = VisualizationTransf2D(logicSpace, viewSpace);

      // Transform mouse coordinates from viewport space to logical space
      float mouseX_logic = (float)(logicSpace.x + (mouseX - viewSpace.x) * (logicSpace.width - logicSpace.x) / (viewSpace.width - viewSpace.x));
      float mouseY_logic = (float)(logicSpace.y + (mouseY - viewSpace.y) * (logicSpace.height - logicSpace.y) / (viewSpace.height - viewSpace.y));

      // Create model matrix
      float dx = mouseX_logic - logicSpace.width / 2;
      float dy = -(logicSpace.height - mouseY_logic - logicSpace.height / 2);
      rotationAngle = atan2(dx, dy);

      // Model matrices generation + Rendering
      // Player
      player->modelMatrix = visMatrix *
          transform2D::Translate(player->translateX, player->translateY) *
          transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
          transform2D::Rotate(rotationAngle) *
          transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);

      RenderEntity(player);

      // Projectile
      for (int i = 0; i < projData.size(); i++) {
          glm::mat3 projectile_modelMatrix = glm::mat3(1);
          projData[i].moveFactor += projSpeedMultiplier * deltaTimeSeconds;

          float tx = cos(glm::pi<float>() * 1.5f + projData[i].rotationAngle) * projData[i].moveFactor;
          float ty = sin(glm::pi<float>() * 1.5f + projData[i].rotationAngle) * projData[i].moveFactor;

          projectile_modelMatrix = visMatrix *
              transform2D::Translate(tx, ty) *
              transform2D::Translate(projData[i].initialPos.x, 
                                     projData[i].initialPos.y) *
              transform2D::Rotate(projData[i].rotationAngle);

          RenderMesh2D(projectile->getMeshes()[0], shaders["VertexColor"], projectile_modelMatrix);
          projData[i].collisionBox.center = glm::vec2(projData[i].initialPos.x + tx, projData[i].initialPos.y + ty);
      }

      // Obstacles
      for (Obstacle* obst : obstacles) {
          obst->modelMatrix = visMatrix;
          RenderEntity(obst);
      }

      // Map
      map->modelMatrix = visMatrix;
      RenderEntity(map);
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_S))
        player->translateY -= deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_W))
        player->translateY += deltaTime * translateSpeed;

    if (window->KeyHold(GLFW_KEY_A))
        player->translateX -= deltaTime * translateSpeed;
    else if (window->KeyHold(GLFW_KEY_D))
        player->translateX += deltaTime * translateSpeed;
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_O) {
        overview_toggle = 1 - overview_toggle;
    }
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
    this->deltaX = deltaX;
    this->deltaY = deltaY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
        // Launch new projectile with current player orientation
        Projectile::ProjectileData proj;
        proj.rotationAngle = this->rotationAngle;
        proj.moveFactor = 0.f;

        float initialPos_x = logicSpace.width / 2 + player->translateX +
          cos(glm::pi<float>() * 1.5f + rotationAngle) * player->largeCircleRadius;
        float initialPos_y = logicSpace.height / 2 + player->translateY +
          sin(glm::pi<float>() * 1.5f + rotationAngle) * player->largeCircleRadius;
        proj.initialPos = glm::vec2(initialPos_x, initialPos_y);

        // Raza cerc coliziune = diagonala proiectil / 2
        proj.collisionBox = { (float)(glm::sqrt(2) * projectile->size / 2),
                              proj.initialPos};
       
        projData.push_back(proj);
    }
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
