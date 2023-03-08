#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


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

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        Mesh* CreateRectangle(
            const std::string& name,
            glm::vec3 center,
            float length,
            float width,
            glm::vec3 color);
        Mesh* CreateTrack(
            const std::string& name,
            float width,
            float height,
            glm::vec3 color);
        Mesh* CreateCube(
            const std::string& name, glm::vec3 color);
        void render_tree(float x, float y);
     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        glm::vec3 carPos;
        float carRot;
        float carSpeed;
        std::vector<float> treex, treey;


        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
