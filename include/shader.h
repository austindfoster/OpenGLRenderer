#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragPath);

    void use();
    bool isValid();
    void setFloat(const std::string &namd, float value);

private:
    unsigned int ID = -1;
};