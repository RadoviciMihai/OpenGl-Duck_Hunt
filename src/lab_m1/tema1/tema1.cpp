#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    window->HidePointer();
    bulets = 0;
    lives = 0;
    speedbase = 1;
    score = 0;
    maxX = resolution.x;
    maxY = resolution.y;
    Mesh* startbutton = object2D::CreateRightangle("startbutton", glm::vec3(0, 0, 0), 300, 200, glm::vec3((float)204/255, 0, 0), true);
    AddMeshToList(startbutton);
    Mesh* startbuttontri = object2D::CreateTriangle("startbuttontri", glm::vec3(100, 50, 0), glm::vec3(200, 100, 0), glm::vec3(100, 150, 0), glm::vec3(1, (float)80/255, (float)80 / 255), true);
    AddMeshToList(startbuttontri);
    Mesh* grassblade = object2D::CreateTriangle("grassblade", glm::vec3(0, 0, 0), glm::vec3(40, 0, 0), glm::vec3(20, 150, 0), glm::vec3(0, 0.7, 0), true);
    AddMeshToList(grassblade);
    Mesh* score = object2D::CreateRightangle("score", glm::vec3(0, 0, 0), 30, 1, glm::vec3(0, 0, 1), true);
    AddMeshToList(score);
    Mesh* scoreline = object2D::CreateRightangle("scoreline", glm::vec3(0, 0, 0), 30, 300, glm::vec3(0, 0, 1), false);
    AddMeshToList(scoreline);

    Mesh* cross = object2D::CreateRightangle("cross", glm::vec3(3, -1, 0), 15, 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(cross);

    Mesh* sky = object2D::CreateRightangle("sky", glm::vec3(0, 0, 0), maxX, maxY, glm::vec3((float)135/255, (float)206/255, (float)235/255), true);
    AddMeshToList(sky);
    addbullet();
    game_started = false;
    makeDuck();

    Mesh* win = new Mesh("win");
    win->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "untitled"), "untitled.fbx");
    meshes[win->GetMeshID()] = win;

    gameWon = false;
    Mesh* heart = object2D::CreateHeart("heart", glm::vec3(1, 0, 0));
    AddMeshToList(heart);

}

float time_acumulator = 0;

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor((float)135 / 255, (float)206 / 255, (float)235 / 255, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    maxX = resolution.x;
    maxY = resolution.y;
    duck->setBounds(maxX, maxY);
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
}

void Tema1::Update(float deltaTimeSeconds)
{
    if(!gameWon) {
        renderBulet();
        renderLives();
    }
    renderCrosshair();
    renderGrass();

    if (gameWon) {
        RenderMesh2D(meshes["win"], shaders["Simple"], transform2D::Translate(maxX/2,maxY/2)* transform2D::Scale(500, 500));
    }
    else {
        if (game_started) {
            if (score >= 300)
                gameWon = true;
            RenderMesh2D(meshes["scoreline"], shaders["VertexColor"], transform2D::Translate(30, 200));
            RenderMesh2D(meshes["score"], shaders["VertexColor"], transform2D::Translate(30, 200) * transform2D::Scale(1, score));
            if (lives == 0) {
                bulets = 0;
                score = 0;
                game_started = false;
            }
            if (duck->getAlive())
                if (duck->getFlightTime() > 0)
                    duck->fly(deltaTimeSeconds);
                else
                    duck->escape(deltaTimeSeconds);
            else
                duck->die(deltaTimeSeconds);
            RenderDuck(duck);
            time_acumulator += deltaTimeSeconds;

            if ((duck->getDuckY() > maxY && duck->getFlightTime() < 0)) {
                makeDuck();
                lives--;
            }
            if (duck->getDuckY() + duck->getSize() < 0 && !duck->getAlive()) {
                makeDuck();
            }

            duckAge += deltaTimeSeconds;
            if (duckAge > 1 && !duckSpawned) setDuck();

        }
        else {
            RenderMesh2D(meshes["startbuttontri"], shaders["VertexColor"], transform2D::Translate(maxX / 2 - 150, maxY / 2 - 100));
            RenderMesh2D(meshes["startbutton"], shaders["VertexColor"], transform2D::Translate(maxX / 2 - 150, maxY / 2 - 100));
        }
    }
    RenderMesh2D(meshes["sky"], shaders["VertexColor"], glm::mat3(1));
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

    if (window->KeyHold(GLFW_KEY_P)) printf("%f %f\n",duck->getDuckX(),duck->getDuckY());
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key==GLFW_KEY_L) lives++;

    if (key == GLFW_KEY_K) score+=30;

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mx = mouseX;
    my = maxY - mouseY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    int x = mouseX;
    int y = maxY - mouseY;
    if (game_started) {
        bulets--;
        if (duck->getAlive()&&bulets>0)
            if (y >= duck->getDuckY() && y <= duck->getDuckY() + duck->getSize()) {
                if (duck->getFlightTime() < 0 || duck->getDuckSpeedX() > 0) {
                    if (x >= duck->getDuckX() && x <= duck->getDuckX() + duck->getSize()) {
                        duck->kill();
                        score += duck->getDuckSpeed() / (duck->getSize()-15);
                    }
                }
                else
                    if (x >= duck->getDuckX() - duck->getSize() && x <= duck->getDuckX()) {
                        duck->kill();
                        score += duck->getDuckSpeed() / (duck->getSize() - 15);
                    }
            }
    }
    else {
        if (x >= maxX / 2 - 150 && y >= maxY / 2 - 100 && x <= maxX / 2 + 150 && y <= maxY / 2 + 100) {
            game_started = true;
            bulets = 6;
            lives = 3;
            speedbase = 1.5;
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

void m1::Tema1::AddDuckToList(Duck* duck)
{
    AddMeshToList(duck->getBeak());
    AddMeshToList(duck->getHead());
    AddMeshToList(duck->getBody());
    AddMeshToList(duck->getInfWing());
    AddMeshToList(duck->getSupWing());
    AddMeshToList(duck->getInfTail());
    AddMeshToList(duck->getSupTail());
   
}

void m1::Tema1::RenderDuck(Duck* duck)
{

    RenderMesh2D(meshes["infWing"], shaders["VertexColor"], duck->getWingTranslation());
    RenderMesh2D(meshes["supWing"], shaders["VertexColor"], duck->getWingTranslation());
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], duck->getDuckTranslation());
    RenderMesh2D(meshes["head"], shaders["VertexColor"], duck->getDuckTranslation());
    RenderMesh2D(meshes["body"], shaders["VertexColor"], duck->getDuckTranslation());
    RenderMesh2D(meshes["infTail"], shaders["VertexColor"], duck->getDuckTranslation());
    RenderMesh2D(meshes["supTail"], shaders["VertexColor"], duck->getDuckTranslation());

}

void m1::Tema1::makeDuck()
{
    duck = new Duck(rngn(20, 35),2+ rngn(40, 70) / 10);
    duck->setBounds(maxX, maxY);
    duckAge = 0;
    duckSpawned = false;
    duck->setDuckSpeedX(0);
    duck->setDuckSpeedY(0);
    duck->moveDuck(-2000, -2000);
    AddDuckToList(duck);
    speedbase += 0.20;
}

void m1::Tema1::setDuck()
{
    duck->setDuckSpeedX(speedbase * rngn(100, 200));
    duck->setDuckSpeedY(speedbase * rngn(100, 200));
    duck->moveDuck(rngn(0, maxX)+ 2000, 2000);
    if (rand() % 2) duck->mirror();
    duckSpawned = true;
    bulets = 6;
}

void m1::Tema1::renderGrass()
{
    for (int i = 0; i < 400; i++) {
        RenderMesh2D(meshes["grassblade"], shaders["VertexColor"], transform2D::Translate(i*10-20,0));
    }
}

void m1::Tema1::renderBulet()
{
    glm::mat3 translation_matrix = transform2D::Scale(0.3, 0.5);
    translation_matrix *= transform2D::Translate(0, 2*maxY - 300);
    for (int i = 0; i < bulets;i++) {
        RenderMesh2D(meshes["b0"], shaders["VertexColor"], translation_matrix);
        RenderMesh2D(meshes["b1"], shaders["VertexColor"], translation_matrix);
        RenderMesh2D(meshes["b2"], shaders["VertexColor"], translation_matrix);
        RenderMesh2D(meshes["b3"], shaders["VertexColor"], translation_matrix);
        translation_matrix *= transform2D::Translate(100, 0);
    }
}

void m1::Tema1::renderLives()
{
    glm::mat3 translation_matrix = transform2D::Translate(0,maxY - 100);
    translation_matrix *= transform2D::Scale(0.7, 0.7);
    for (int i = 0; i < lives; i++) {
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], translation_matrix);
        translation_matrix *= transform2D::Translate(110, 0);
    }
}

void m1::Tema1::addbullet()
{

    Mesh* b0 = object2D::CreateTriangle("b0", glm::vec3(30, 0, 0), glm::vec3(20, 50, 0), glm::vec3(0, 30, 0), glm::vec3(0.8, 0.2, 0.2), true);
    AddMeshToList(b0);
    Mesh* b1 = object2D::CreateTriangle("b1", glm::vec3(30, 0, 0), glm::vec3(50, 20, 0), glm::vec3(20, 50, 0), glm::vec3(0.8, 0.2, 0.2), true);
    AddMeshToList(b1);
    Mesh* b2 = object2D::CreateTriangle("b2", glm::vec3(50, 20, 0), glm::vec3(70, 100, 0), glm::vec3(20, 50, 0), glm::vec3(0.8, 0.8, 0.2), true);
    AddMeshToList(b2);
    Mesh* b3 = object2D::CreateTriangle("b3", glm::vec3(50, 20, 0), glm::vec3(100, 70, 0), glm::vec3(70, 100, 0), glm::vec3(0.8, 0.8, 0.2), true);
    AddMeshToList(b3);
}

void m1::Tema1::renderCrosshair()
{
    RenderMesh2D(meshes["cross"], shaders["VertexColor"],transform2D::Translate(mx,my)* transform2D::Rotate(0.f * M_PI));
    RenderMesh2D(meshes["cross"], shaders["VertexColor"],transform2D::Translate(mx,my)* transform2D::Rotate(0.5 * M_PI));
    RenderMesh2D(meshes["cross"], shaders["VertexColor"],transform2D::Translate(mx,my)* transform2D::Rotate(1.f * M_PI));
    RenderMesh2D(meshes["cross"], shaders["VertexColor"],transform2D::Translate(mx,my)* transform2D::Rotate(1.5 * M_PI));
}

float m1::Tema1::rngn(int mi, int ma)
{
    return (rand() % (mi - ma + 1)) + mi;
}
