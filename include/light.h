#pragma once

#include <glm/glm.hpp>

class Light
{
public:
    Light();
    
    glm::vec3 color;
    glm::vec3 position;
};