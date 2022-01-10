#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/player.h"
#include "lab_m1/Tema2/maze.h"
#include "lab_m1/Tema2/wall.h"
#include "lab_m1/Tema2/enemy.h"
#include "lab_m1/Tema2/projectile.h"
#include "lab_m1/Tema2/collision.h"
using namespace tema2;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderEntity(tema2::Entity* entity, const char *shader, float time);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& projMatrix, float elapsedTime);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void CreateCube(const char *name, glm::vec3 color);
        void InitMaze();
        void SpawnProjectile();
        void UpdateCameraPos();
        void UpdatePlayer();
        void UpdateWalls();
        void UpdateEnemies(float deltaTimeSeconds);
        void UpdateCrosshair();
        void UpdateBar(float metric, float height);
        void UpdateProjectiles(float deltaTimeSeconds);
        bool DetectCollision_PlayerWalls();
        bool DetectCollision_EnemyWalls(tema2::Enemy *enemy);
        bool DetectCollision_ProjWalls(tema2::Projectile* proj);
        void HandleCollisions_ProjectiesEnemies();
        void HandleCollision_PlayerEnemies();
        void ChangePerspective();

        tema2::Player* player;
        tema2::Maze* maze;
        std::vector<tema2::Wall*> walls;
        std::vector<tema2::Enemy*> enemies;
        std::vector<tema2::Projectile*> projectiles;

        glm::mat4 projectionMatrix;
        glm::mat4 projectionMatrixOrtho;
        glm::mat4 projectionMatrixPersp;

        float timeRemaining;

        bool renderPlayer;
        bool firstPersonCamera;

        float FOV_angle;
        float ortho_x;
        float ortho_y;

        float playerRotateAngle = 0;
    protected:
        Camera* camera;
        Camera* hudCamera;
    };
}   // namespace m1
