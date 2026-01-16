#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <material.h>
#include <texture.h>

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

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList, Material mat, std::vector<Texture> textureList, Shader *shader = nullptr);

    void draw();
    void setShader(Shader *shdr);
    void buildOptimalShader();


    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    Shader *shader;
    std::vector<Texture> textures;
    unsigned int VAO;

private:
    void setupMesh();

    // render data
    unsigned int VBO, EBO;
};
