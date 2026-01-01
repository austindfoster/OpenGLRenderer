#include <mesh.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

void Mesh::setVertices(const std::vector<float> vertices)
{
    m_vertices = vertices;
}

void Mesh::addShader(Shader &shader)
{
    m_shaders.push_back(shader);
}
