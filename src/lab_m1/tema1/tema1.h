#pragma once

#include "components/simple_scene.h"
#include "duck.h"

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
        void AddDuckToList(Duck* duck);
        void RenderDuck(Duck* duck);
        void makeDuck();
        void setDuck();
        void renderGrass();
        void renderBulet();
        void renderLives();
        void addbullet();
        void renderCrosshair();
        float rngn(int, int);



     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float duckAge;
        Duck* duck;
        bool game_started;
        int lives;
        int bulets;
        float speedbase;
        float score;
        int mx, my;
        int maxX, maxY;
        bool duckSpawned;
        bool gameWon;
        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
