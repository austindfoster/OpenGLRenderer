#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Color {
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
    void setColor(const std::string &name, Color value);

private:
    unsigned int ID = -1;
};