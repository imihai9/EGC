#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

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

    //GetSceneCamera()->SetOrthographic(left + tx, right - tx, bottom + ty, top - ty, 0.1f, 400);
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
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = GetSceneCamera();

    // Transform translation factors from logical space to viewport space (in order to keep player centered
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


      // Check for collisions
      if (CheckCollisionRectCircleInside(map->getCollisionBox(), player->getCollisionBox()))
          player->translateX = 0;

      // Create model matrix
      float dx = mouseX_logic - logicSpace.width / 2;
      float dy = -(logicSpace.height - mouseY_logic - logicSpace.height / 2);
      rotationAngle = atan2(dx, dy);

      player->modelMatrix = visMatrix *
          transform2D::Translate(player->translateX, player->translateY) *
          transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) *
          transform2D::Rotate(rotationAngle) *
          transform2D::Translate(-logicSpace.width / 2, -logicSpace.height / 2);


    RenderEntity(player);
    RenderEntity(map);
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
