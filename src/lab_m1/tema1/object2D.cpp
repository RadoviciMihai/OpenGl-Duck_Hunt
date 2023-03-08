#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"


Mesh* object2D::CreateRightangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    float heigth,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, heigth, 0), color),
        VertexFormat(corner + glm::vec3(0, heigth, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 Corner1,
    glm::vec3 Corner2,
    glm::vec3 Corner3,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(Corner1, color),
        VertexFormat(Corner2, color),
        VertexFormat(Corner3, color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2};

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateHeart(const std::string& name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(50, 50, 0), color)
    };

    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(0, doublecircle(0),0), color));
    for (int x = 1; x <= 100; x++) {
        vertices.push_back(VertexFormat(glm::vec3(x,doublecircle(x),0),color));
        indices.push_back(0);
        indices.push_back(x);
        indices.push_back(x+1);
    }
    vertices.push_back(VertexFormat(glm::vec3(0, doublecircle(0), 0), color));
    for (int x = 0; x <= 100; x++) {
        vertices.push_back(VertexFormat(glm::vec3(x, bottompart(x), 0), color));
        indices.push_back(0);
        indices.push_back(103+x);
        indices.push_back(102+x);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

float object2D::doublecircle(float x)
{
    return 25*(M_PI+sqrt(1-pow(abs(x-50)/25-1, 2)));
}

float object2D::bottompart(float x)
{
    return 25*acos(1-abs(x-50)/25);
}
