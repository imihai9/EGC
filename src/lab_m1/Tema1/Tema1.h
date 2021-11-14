#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/space.h"
#include "lab_m1/Tema1/map.h"
#include "lab_m1/Tema1/player.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

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

        void InitEntity(Entity* entity);
        void RenderEntity(Entity* entity);

        // Viewport related
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        glm::mat3  visMatrix;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float scaleX, scaleY;
        float rotationAngle;
        int mouseX, mouseY;
        int deltaX;
        int deltaY;
        float resize_factor;
        int overview_toggle; // Toggles between showing whole map / a part of it;  For debug purposes

        Player* player;
        Map* map;
    };

}   // namespace m1


