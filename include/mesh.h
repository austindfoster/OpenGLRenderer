#pragma once

#include <vector>
#include <shader.h>
#include <glm/glm.hpp>

class Mesh {
public:
    void setVertices(const std::vector<float> vertices);
    void addShader(Shader &shader);

    void draw();
    
    bool hasTexture();

    std::vector<float> m_vertices;
    std::vector<Shader> m_shaders;
    std::vector<glm::vec3> m_instances;
};