#include <light.h>
#include <GLFW/glfw3.h>

Light::Light()
{
    position = glm::vec3{0.5f, 1.0f, 0.5f};
    color = glm::vec3{1.0f, 1.0f, 1.0f};
}