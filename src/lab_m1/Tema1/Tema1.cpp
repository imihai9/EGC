#include "lab_m1/Tema1/Tema1.h"
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

float random(float min, float max) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;
    return (random * range) + min;
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

    lastEnemyWaveTime = -1.f;
    lastProjectileLaunchTime = -1.f;
    playerSpeed = 150.f;
    projSpeedMultiplier = 250.f;
    playerHealth = 100.f;
    playerScore = 0.f;
    enemyCollisionDmg = 20.f;
    maxRangeSquared = 150.f * 150.f;
    fireRate = 0.5f;
    
    // Player instantiation
    player = new Player(logicSpace);
    InitEntity(player);

    // Enemy object instantiation
    enemy = new Enemy(logicSpace);
    InitEntity(enemy);

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

    // Projectile object instantiation
    projectile = new Projectile(logicSpace);
    InitEntity(projectile);

    // Healthbar instantiation
    bar = new Bar(logicSpace);
    InitEntity(bar);

    // Pickup instantiation
    pickup = new Pickup(logicSpace);
    InitEntity(pickup);
}

void Tema1::SpawnPickup(Enemy::EnemyData deadEnemy) {
    int chance = rand() % 21;
    if (chance == 1 || chance == 2) {
        Pickup::PickupData newPickup;
        newPickup.pos = deadEnemy.currentPos;
        newPickup.collisionBox = { deadEnemy.currentPos.x - 30, deadEnemy.currentPos.y - 30, 60, 60 };
        pickupData.push_back(newPickup);
    }
}

void Tema1::HandleCollisions() {
    { // {Player - Enemy}
        vector<Enemy::EnemyData>::iterator it;
        for (it = enemyData.begin(); it < enemyData.end(); ) {
            bool collided = CheckCollisionCircleCircle (&(*it).collisionBox, player->getCollisionBox());
            if (collided) {
                playerHealth -= enemyCollisionDmg;
                it = enemyData.erase(it);
            }
            else {
                it++;
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

    { // {Projectile - Enemy}
        vector<Projectile::ProjectileData>::iterator it_p;
        vector<Enemy::EnemyData>::iterator it_e;
        for (it_p = projData.begin(); it_p < projData.end(); ) { // for each projectile
            bool collided = false;
            for (it_e = enemyData.begin(); it_e < enemyData.end(); ) { // for each enemy
                collided = CheckCollisionCircleCircle(&(*it_p).collisionBox, &(*it_e).collisionBox);
                if (collided) {
                    playerScore++;
                    if (playerScore == 200)
                        playerScore = 0;
                    SpawnPickup(*(it_e));
                    it_e = enemyData.erase(it_e);
                    it_p = projData.erase(it_p);
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

    { // {Player - Pickup}
        vector<Pickup::PickupData>::iterator it;
        for (it = pickupData.begin(); it < pickupData.end(); ) {
            glm::vec2 penetration = CheckCollisionRectCircle(&(*it).collisionBox, player->getCollisionBox());
            if (penetration != glm::vec2(0)) {
                playerHealth = min(100.f, playerHealth + 20.f);
                it = pickupData.erase(it);
            }
            else {
                it++;
            }
        }
    }

    { // {Enemy - Map walls}
        vector<Enemy::EnemyData>::iterator it;
        for (it = enemyData.begin(); it < enemyData.end(); it++) {
            glm::vec2 penetration = CheckCollisionRectCircleInside(map->getCollisionBox(), &(*it).collisionBox);
            if (penetration != glm::vec2(0)) {
                (*it).currentPos += penetration;
            }
        }
    }
}

void Tema1::FrameStart()
{
    HandleCollisions();

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    // Camera setup
    auto camera = GetSceneCamera();

    // Transform translation factors from logical space to viewport space (in order to keep player centered)
    float tx_view = (float)(viewSpace.x + (viewSpace.width - viewSpace.x) * (player->translateX - logicSpace.x) / (logicSpace.width - logicSpace.x));
    float ty_view = (float)(viewSpace.y + (viewSpace.height - viewSpace.y) * (player->translateY - logicSpace.y) / (logicSpace.height - logicSpace.y));

    camera->SetPosition(glm::vec3(tx_view, ty_view, 50));
    camera->Update();
    GetCameraInput()->SetActive(false);
}

void Tema1::UpdatePlayer() {
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
}

void Tema1::UpdateHealthbar() {
    glm::mat3 maxHealthModelMatrix = visMatrix *  // follow player (in order to stay fixed on screen
        transform2D::Translate(logicSpace.width / 2 + 350, logicSpace.height / 2 + 200) *
        transform2D::Translate(player->translateX, player->translateY);

    glm::mat3 currHealthModelMatrix = maxHealthModelMatrix *
        transform2D::Scale((float)(playerHealth / 100), 1);

    RenderMesh2D(bar->currHealth, shaders["VertexColor"], currHealthModelMatrix);
    RenderMesh2D(bar->maxHealth, shaders["VertexColor"], maxHealthModelMatrix);
}

void Tema1::UpdateScorebar() {
    glm::mat3 maxScoreModelMatrix = visMatrix *  // follow player (in order to stay fixed on screen
        transform2D::Translate(logicSpace.width / 2 + 350, logicSpace.height / 2 + 150) *
        transform2D::Translate(player->translateX, player->translateY);

    glm::mat3 currScoreModelMatrix = maxScoreModelMatrix *
        transform2D::Scale((float)(playerScore / 200), 1);

    RenderMesh2D(bar->currHealth, shaders["VertexColor"], currScoreModelMatrix);
    RenderMesh2D(bar->maxHealth, shaders["VertexColor"], maxScoreModelMatrix);
}

void Tema1::UpdateEnemies(float deltaTimeSeconds) {
    glm::mat3 enemyModelMatrix = glm::mat3(1);
    for (int i = 0; i < enemyData.size(); i++) {
        // -- player - enemy vector
        // -- rotation
        float dx_rot = (logicSpace.width / 2 + player->translateX) - enemyData[i].currentPos.x;
        float dy_rot = (logicSpace.height / 2 + player->translateY) - enemyData[i].currentPos.y;
        enemyData[i].rotationAngle = atan2(dx_rot, -dy_rot);

        // -- translation
        float tx = cos(glm::pi<float>() * 1.5f + enemyData[i].rotationAngle);
        float ty = sin(glm::pi<float>() * 1.5f + enemyData[i].rotationAngle);

        enemyData[i].currentPos.x += tx * enemyData[i].speedMultiplier * deltaTimeSeconds;
        enemyData[i].currentPos.y += ty * enemyData[i].speedMultiplier * deltaTimeSeconds;

        enemyModelMatrix = visMatrix *
            transform2D::Translate(enemyData[i].currentPos.x, enemyData[i].currentPos.y) *
            transform2D::Rotate(enemyData[i].rotationAngle);

        vector<Mesh*> enemyMeshes = enemy->getMeshes();
        for (int i = 0; i < enemyMeshes.size(); i++) {
            RenderMesh2D(enemyMeshes[i], shaders["VertexColor"], enemyModelMatrix);
        }

        enemyData[i].collisionBox.center = glm::vec2(enemyData[i].currentPos.x, enemyData[i].currentPos.y);
    }

    // --- Spawn new enemy wave (5 randomly placed enemies) at start & every 8s
    float elapsedTime = Engine::GetElapsedTime();

    if (lastEnemyWaveTime == -1.f || elapsedTime - lastEnemyWaveTime >= 8.f) {
        lastEnemyWaveTime = elapsedTime;
        Enemy::EnemyData newEnemyData;

        float player_pos_x = player->translateX + logicSpace.width / 2;
        float player_pos_y = player->translateY + logicSpace.height / 2;

        for (int i = 0; i < 5; i++) {
            RectCB* map_cb = map->getCollisionBox();
            float initial_x, initial_y;

            // Don't spawn around player
            do {
                initial_x = random(map_cb->pos_x - enemy->initial_tx * glm::sqrt(2),
                    map_cb->width);

                initial_y = random(map_cb->pos_y - enemy->initial_ty * glm::sqrt(2),
                    map_cb->height);
            } while (abs(initial_x - player_pos_x) < 100 && abs(initial_y - player_pos_y) < 100);

            newEnemyData.initialPos = glm::vec2(initial_x, initial_y); // spawn point
            newEnemyData.currentPos = newEnemyData.initialPos;

            int speedyEnemyChance = rand() % 20;
            if (speedyEnemyChance == 1)
                newEnemyData.speedMultiplier = 350.f;
            else
                newEnemyData.speedMultiplier = random(50.f, 140.f);

            newEnemyData.collisionBox.radius = (float)(25 * glm::sqrt(2));
            enemyData.push_back(newEnemyData);
        }
    }
}


void Tema1::UpdateProjectiles(float deltaTimeSeconds) {
    vector<Projectile::ProjectileData>::iterator it;

    for (it = projData.begin(); it < projData.end(); ) {
        glm::mat3 projectile_modelMatrix = glm::mat3(1);
        (*it).moveFactor += projSpeedMultiplier * deltaTimeSeconds;

        float tx = cos(glm::pi<float>() * 1.5f + (*it).rotationAngle) * (*it).moveFactor;
        float ty = sin(glm::pi<float>() * 1.5f + (*it).rotationAngle) * (*it).moveFactor;

        glm::vec2 projectile_currPos = (*it).initialPos + glm::vec2(tx, ty);

        float dx = projectile_currPos.x - (*it).initialPos.x;
        float dy = projectile_currPos.y - (*it).initialPos.y;

        // Destroy projectile if max range reached
        if (dx * dx + dy * dy > maxRangeSquared) {
            it = projData.erase(it);
        }

        else {
            projectile_modelMatrix = visMatrix *
                transform2D::Translate(projectile_currPos.x, projectile_currPos.y) *
                transform2D::Rotate((*it).rotationAngle);

            RenderMesh2D(projectile->getMeshes()[0], shaders["VertexColor"], projectile_modelMatrix);
            (*it).collisionBox.center = glm::vec2(projectile_currPos.x, projectile_currPos.y);
            it++;
        }
    }
}

void Tema1::UpdateObstacles() {
    for (Obstacle* obst : obstacles) {
        obst->modelMatrix = visMatrix;
        RenderEntity(obst);
    }
}

void Tema1::UpdatePickups() {
    for (Pickup::PickupData pickupDataObj : pickupData) {
        glm::mat3 modelMatrix = visMatrix *
            transform2D::Translate(pickupDataObj.pos.x, pickupDataObj.pos.y);

       RenderMesh2D(pickup->getMeshes()[0], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::UpdateMap() {
    map->modelMatrix = visMatrix;
    RenderEntity(map);
}


void Tema1::GameOver() { 
    // Should probably draw something here
}

void Tema1::Update(float deltaTimeSeconds)
{
      glm::ivec2 resolution = window->GetResolution();

      // Sets the screen area where to draw
      viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
      SetViewportArea(viewSpace, glm::vec3(0), true);

      // Check is game is over
      if (playerHealth <= 0.f) {
          GameOver();
      }
      else {
          // Compute the 2D visualization matrix
          visMatrix = VisualizationTransf2D(logicSpace, viewSpace);

          UpdatePlayer();
          UpdateHealthbar();
          UpdateScorebar();
          UpdateEnemies(deltaTimeSeconds);
          UpdateProjectiles(deltaTimeSeconds);
          UpdateObstacles();
          UpdatePickups();
          UpdateMap();
      }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_S))
        player->translateY -= deltaTime * playerSpeed;
    else if (window->KeyHold(GLFW_KEY_W))
        player->translateY += deltaTime * playerSpeed;

    if (window->KeyHold(GLFW_KEY_A))
        player->translateX -= deltaTime * playerSpeed;
    else if (window->KeyHold(GLFW_KEY_D))
        player->translateX += deltaTime * playerSpeed;
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_O) {
        overview_toggle = 1 - overview_toggle;
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    this->deltaX = deltaX;
    this->deltaY = deltaY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
        float elapsedTime = Engine::GetElapsedTime();
        if (lastProjectileLaunchTime == -1.f || elapsedTime - lastProjectileLaunchTime >= fireRate) {

            lastProjectileLaunchTime = elapsedTime;
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
                                  proj.initialPos };

            projData.push_back(proj);
        }
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
