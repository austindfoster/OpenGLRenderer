#pragma once

#include <glm/glm.hpp>

struct Material
{
    unsigned int id;
    glm::vec3 diffuse = {0.800000f, 0.077166f, 0.737119f};
    glm::vec3 specular = {0.5f, 0.5f, 0.5f};
    glm::vec3 ambient = {1.0f, 1.0f, 1.0f};
    glm::vec3 emmisive = {0.0f, 0.0f, 0.0f};
    glm::vec3 transparent;
    glm::vec3 reflective;
};
