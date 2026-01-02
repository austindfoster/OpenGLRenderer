#include <light.h>
#include <GLFW/glfw3.h>

Light::Light()
{
    position = glm::vec3{0.5f, 1.0f, 0.0f};
    color = glm::vec3{1.0f, 1.0f, 1.0f};
    // color.r = static_cast<float>(sin(glfwGetTime() * 2.0));
    // color.g = static_cast<float>(sin(glfwGetTime() * 0.7));
    // color.b = static_cast<float>(sin(glfwGetTime() * 1.3));
}