#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;
    carPos = glm::vec3(0, 0.25f, 1.3f);
    camera = new implemented::Camera();
    carRot = 0;
    carSpeed = 0;
    camera->Set(glm::vec3(0, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    for (int i = 0; i < 15; i++) {
        float x = (float)(rand() % 60 - 30);
        float y = (float)(rand() % 60 - 30);
        treex.push_back(x);
        treey.push_back(y);
    }

    {
        Mesh* mesh = CreateCube("box", glm::vec3(0.0f, 0.8f, 0.8f));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = CreateRectangle("grass", glm::vec3(0, 0, 0), 40, 40, glm::vec3(0.1f, 0.5f, 0.1f));
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = CreateTrack("road", 0.5f, 0.1f, glm::vec3(0.2f, 0.2f, 0.2f));
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = CreateCube("leaf",glm::vec3(0.2f, 0.9f, 0.2f));
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = CreateCube("stem", glm::vec3(0.4f, 0.2f, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    //printf("%f %f %f\n",camera->forward)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, carPos);
        modelMatrix = glm::rotate(modelMatrix, carRot, glm::vec3(0,1,0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f,0.5f,1));
        RenderMesh(meshes["box"], shaders["VertexColor"], modelMatrix);
    }

        RenderMesh(meshes["grass"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh(meshes["road"], shaders["VertexColor"], glm::mat3(1));

    for (int i = 0; i < 15; i++) {

        render_tree(treex[i], treey[i]);
    }
    float deltastep = deltaTimeSeconds * carSpeed;
    carPos += glm::vec3(camera->forward[0] * deltastep, 0, camera->forward[2] * deltastep);
    camera->position += glm::vec3(camera->forward[0] * deltastep, 0, camera->forward[2] * deltastep);
    if (carSpeed > 0)
    {
        carSpeed -= deltaTimeSeconds;
        camera->RotateThirdPerson_OX(-0.015f * deltaTimeSeconds);
    }
    else
    {
        carSpeed += deltaTimeSeconds;
        camera->RotateThirdPerson_OX(0.015f * deltaTimeSeconds);
    }

}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->RotateThirdPerson_OY(deltaTime);
            carRot += deltaTime;

        }

        if (window->KeyHold(GLFW_KEY_W)) {
            if (carSpeed<15)
            {
                carSpeed += deltaTime * 3;
                camera->RotateThirdPerson_OX(0.045f*deltaTime);
            }
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            if (carSpeed>-5)
            {
                carSpeed -= deltaTime * 3;
                camera->RotateThirdPerson_OX(-0.045f * deltaTime);
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->RotateThirdPerson_OY(-deltaTime);
            carRot -= deltaTime;

        }
        /*
        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

    */// TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

}


void Tema2::OnKeyPress(int key, int mods)
{
    static float fov = 90.f;

    static float h = 30.f;
    static float w = 30.f;
    /*
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_P) projectionMatrix = glm::perspective(fov, 2.f, 2.f, 200.0f);
    if (key == GLFW_KEY_O) projectionMatrix = glm::ortho(-h, h, -w, w, 1.f,50.0f);
    if (key == GLFW_KEY_N) {
        fov += 0.1; 
        projectionMatrix = glm::perspective(fov, 2.f, 2.f, 200.0f);
    }
    if (key == GLFW_KEY_M) {
        fov -= 0.1;
        projectionMatrix = glm::perspective(fov, 2.f, 2.f, 200.0f);
    }
    if (key == GLFW_KEY_K) {
        h += 2.f;
        projectionMatrix = glm::ortho(-h, h, -w, w, 1.f, 50.0f);
    }
    if (key == GLFW_KEY_L) {
        w += 2.f;
        projectionMatrix = glm::ortho(-h, h, -w, w, 1.f, 50.0f);
    }

    if (key == GLFW_KEY_H) {
        h -= 2.f;
        projectionMatrix = glm::ortho(-h, h, -w, w, 1.f, 50.0f);
    }
    if (key == GLFW_KEY_J) {
        w -= 2.f;
        projectionMatrix = glm::ortho(-h, h, -w, w, 1.f, 50.0f);
    }
    */
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        /*
        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
            camera->RotateFirstPerson_OX(-sensivityOY * deltaY);
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.

        }*/

        if (window->GetSpecialKeyState()) {
            renderCameraTarget = true; 
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
            camera->RotateThirdPerson_OX(-sensivityOY * deltaY);
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.

        }
    }
}


Mesh* Tema2::CreateRectangle(
    const std::string& name,
    glm::vec3 center,
    float length,
    float width,
    glm::vec3 color)
{
    glm::vec3 corner = center;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-length, 0, -width), color),
        VertexFormat(corner + glm::vec3(-length, 0, width), color),
        VertexFormat(corner + glm::vec3(length, 0, width), color),
        VertexFormat(corner + glm::vec3(length, 0, -width), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    indices.push_back(0);
    indices.push_back(2);

    square->InitFromData(vertices, indices);
    return square;
}
float fdpm2(float x) {
    return pow(abs(30*cos(x)-2*x) / 25 + 0.5f, -2) / 10;
}

float curvef(float x) {
    return 1.2f * sin(x) + 3 - (x * x / 25);
}


Mesh* Tema2::CreateTrack(
    const std::string& name,
    float width,
    float height,
    glm::vec3 color)
{
    std::vector<glm::vec3> contour;
    std::vector<VertexFormat> vertices;
    /*
    contour.push_back(glm::vec3(-70, height, 0));
    for (float x=-7;x<=9;x+=fdpm2(x))
    {
        contour.push_back(glm::vec3(x*10, height, 10*curvef(x)));
    }

    contour.push_back(glm::vec3(90, height, -5));

    contour.push_back(glm::vec3(90, height, 0));

    contour.push_back(glm::vec3(90, height, 5));
    for (float x = 9; x >= -7; x -= fdpm2(x))
        contour.push_back(glm::vec3(x*10, height, -curvef(x)*10));

    contour.push_back(glm::vec3(-70, height, 0));
    */


    contour.push_back(glm::vec3(-70, height, 7));
    contour.push_back(glm::vec3(-67, height, 11));
    contour.push_back(glm::vec3(-64, height, 15));
    contour.push_back(glm::vec3(-61, height, 18));
    contour.push_back(glm::vec3(-60, height, 23));
    contour.push_back(glm::vec3(-57, height, 27));
    contour.push_back(glm::vec3(-54, height, 30));
    contour.push_back(glm::vec3(-51, height, 32));
    contour.push_back(glm::vec3(-47, height, 33));
    contour.push_back(glm::vec3(-43, height, 32));
    contour.push_back(glm::vec3(-39, height, 30));
    contour.push_back(glm::vec3(-36, height, 24));
    contour.push_back(glm::vec3(-33, height, 21));
    contour.push_back(glm::vec3(-29, height, 17));
    contour.push_back(glm::vec3(-20, height, 17));
    contour.push_back(glm::vec3(-15, height, 19));
    contour.push_back(glm::vec3(-10, height, 27));
    contour.push_back(glm::vec3(-6 , height, 26));
    contour.push_back(glm::vec3(-2 , height, 32));
    contour.push_back(glm::vec3(-2 , height, 37));
    contour.push_back(glm::vec3( 6 , height, 39));
    contour.push_back(glm::vec3( 10, height, 41));
    contour.push_back(glm::vec3( 20, height, 39));
    contour.push_back(glm::vec3( 24, height, 35));
    contour.push_back(glm::vec3( 26, height, 32));
    contour.push_back(glm::vec3( 29, height, 28));
    contour.push_back(glm::vec3( 33, height, 23));
    contour.push_back(glm::vec3( 36, height, 18));
    contour.push_back(glm::vec3( 40, height, 13));
    contour.push_back(glm::vec3( 44, height, 10));
    contour.push_back(glm::vec3( 49, height, 9));
    contour.push_back(glm::vec3(54, height, 9));
    contour.push_back(glm::vec3( 59, height, 12));
    contour.push_back(glm::vec3( 66, height, 16));
    contour.push_back(glm::vec3( 73, height, 18));
    contour.push_back(glm::vec3( 80, height, 16));
    contour.push_back(glm::vec3( 84, height, 12));
    contour.push_back(glm::vec3( 86, height, 8));
    contour.push_back(glm::vec3( 89, height, -3));
    contour.push_back(glm::vec3( 92, height, -3));
    contour.push_back(glm::vec3( 92, height, -7));
    contour.push_back(glm::vec3(88, height, -13));
    contour.push_back(glm::vec3(79, height, -15));
    contour.push_back(glm::vec3(64, height, -18));
    contour.push_back(glm::vec3(48, height, -18));
    contour.push_back(glm::vec3(32, height, -18));
    contour.push_back(glm::vec3(0, height, -20));
    contour.push_back(glm::vec3(-20, height, -20));
    contour.push_back(glm::vec3(-40, height, -17));
    contour.push_back(glm::vec3(-51, height, -13));
    contour.push_back(glm::vec3(-60, height, -9));
    contour.push_back(glm::vec3(-70, height, -3));
    contour.push_back(glm::vec3(-70, height, 7));

    for(int i=0;i<contour.size()-1;i++)
    {
        
        glm::vec3 d = contour[i + 1] - contour[i];
        glm::vec3 p = glm::cross(glm::vec3(0, 1, 0),d);
        glm::vec3 r = contour[i] + width * p;
        glm::vec3 a = contour[i] - width * p;
        vertices.push_back(VertexFormat(a, color));
        vertices.push_back(VertexFormat(r, color));
    }

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i =0; i < contour.size() - 1; i++) {
        indices.push_back(2 * i);
        indices.push_back(2 * (i+1));
        indices.push_back(2 * i+ 1);
        indices.push_back(2 * (i + 1));
        indices.push_back(2 * (i + 1) + 1);
        indices.push_back(2 * i + 1);
    }

    //square->SetDrawMode(GL_LINE_LOOP);
    square->InitFromData(vertices, indices);
    return square;
}


Mesh* Tema2::CreateCube(
    const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(glm::vec3(0.5, 0.5, 0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(0.5, 0.5, -0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(0.5, -0.5, 0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(0.5, -0.5, -0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(-0.5, 0.5, 0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(-0.5, 0.5, -0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(-0.5, -0.5, 0.5), color));
    vertices.push_back(VertexFormat(glm::vec3(-0.5, -0.5, -0.5), color)); 
    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 2, 1, 7, 3, 1, 7, 1, 5, 3, 7, 6, 3,
    6, 2,7,5,6,4,6,5,1,0,4,1,4,5,2,4,0,2,6,4};
    square->InitFromData(vertices, indices);
    return square;
}

void Tema2::render_tree(float x, float y) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 1.5f, y));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 3.0f, 0.5f));
        RenderMesh(meshes["stem"], shaders["VertexColor"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix1 = glm::mat4(1);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(x, 2.7f, y));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(2, 2, 2));
        RenderMesh(meshes["leaf"], shaders["VertexColor"], modelMatrix1);
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}




void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
