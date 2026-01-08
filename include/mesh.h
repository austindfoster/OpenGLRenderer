#pragma once

#include <vector>
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

class Shader;

struct Vertex
{
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
    // bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};

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

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList, Material mat, std::vector<Texture> textureList);

    void draw(Shader &shader);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    std::vector<Texture> textures;
    unsigned int VAO;

private:
    void setupMesh();

    // render data
    unsigned int VBO, EBO;
};