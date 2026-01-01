#pragma once

#include <texture.h>

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
    void setColor(const std::string &name, Color value);
    void setTransform(const std::string &name, glm::mat4 matrix);

    int addTexture(const char * path, bool hasAlpha = false);
    void bindTextures();

private:
    std::vector<Texture> m_textures;
    unsigned int ID = -1;
};