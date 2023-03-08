#pragma once

#include <string>
#include <math.h>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateRightangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 Corner1, glm::vec3 Corner2, glm::vec3 Corner3, glm::vec3 color, bool fill);
    Mesh* CreateHeart(const std::string& name, glm::vec3 color);
    float doublecircle(float x);
    float bottompart(float x);
}
