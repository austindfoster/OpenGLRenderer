#include "material.h"
#include <mesh.h>

#include <shader.h>
#include <customShader.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>

constexpr std::string SHADERS_DIR = "src/shaders/";
constexpr std::string DEFAULT_VERTEX_SHADER = "masks.vs";

Mesh::Mesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList, Material mat, std::vector<Texture> textureList, Shader *shader) : vertices(vertexList), indices(indexList), material(mat), textures(textureList), shader(shader)
{
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

void Mesh::draw()
{
    if (!shader) {
        std::cerr << "No shader available on mesh" << std::endl;
        return;
    }
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    shader->setVec3("material.diffuse", material.diffuse);
    shader->setVec3("material.specular", material.specular);
    shader->setVec3("material.ambient", material.ambient);
    if (!textures.empty())
    {
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if (name == "material.diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "material.specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "material.normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "material.height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }
    shader->setFloat("material.shininess", 32.0f);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setShader(Shader *shdr) {
    shader = shdr;
}

void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, boneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, weights));
    glBindVertexArray(0);
}

void Mesh::buildOptimalShader() {
    std::string vertexShaderFileName = SHADERS_DIR + DEFAULT_VERTEX_SHADER;
    std::string customFileName = writeCustomShader(material, textures, true);
    std::vector<std::string> fileNames = getFilesInDirectory(SHADERS_DIR);
    bool isIdentical = false;
    for (auto fileName : fileNames) {
        isIdentical = compareFiles(SHADERS_DIR + fileName, customFileName);
        if (isIdentical) {
            shader = new Shader(vertexShaderFileName.c_str(), (SHADERS_DIR + fileName).c_str());
            removeShaderFile(customFileName);
            return;
        }
    }
    shader = new Shader(vertexShaderFileName.c_str(), customFileName.c_str());
}
