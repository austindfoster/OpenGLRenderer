#pragma once

#include <glm/glm.hpp>
#include <string>

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragPath);
    ~Shader();

    void use();
    bool isValid();
    void setFloat(const std::string &name, float value);
    void setInt(const std::string &name, int value);
    void setVec3(const std::string &name, glm::vec3 value);
    void setTransform(const std::string &name, glm::mat4 matrix);

    int addTexture(unsigned int textureID);
    void bindTextures();

    std::vector<unsigned int> m_textureIDs;
    unsigned int ID = -1;
};