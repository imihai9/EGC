#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/player.h"

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

        //void InitEntity(tema2::Entity* entity);
        void RenderEntity(tema2::Entity* entity);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void CreateCube(const char *name, glm::vec3 color);

    protected:
        Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        bool firstPersonCamera;

        float FOV_angle;
        float ortho_x;
        float ortho_y;

        float cameraRotateAngle = 0;
        tema2::Player* player;
    };
}   // namespace m1
