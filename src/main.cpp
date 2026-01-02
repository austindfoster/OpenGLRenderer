#include "config.h"
#include <camera.h>
#include <light.h>
#include <loadTextures.h>
#include <model.h>
#include <modelinfo.h>
#include <shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fs = std::filesystem;

const uint SCRN_WIDTH = 800;
const uint SCRN_HEIGHT = 600;
const glm::vec3 CAMERA_START_POS = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 CAMERA_POS = CAMERA_START_POS;
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime, Camera &camera);

glm::mat4 circleCameraAroundPoint(float radius, glm::vec3 point)
{
    float camX = static_cast<float>(sin(glfwGetTime()) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
    return glm::lookAt(glm::vec3(camX, 0.0f, camZ), point, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 circleCameraAroundCenter(float radius)
{
    return circleCameraAroundPoint(radius, glm::vec3(0.0f, 0.0f, 0.0f));
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "My Window", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "glad failed to load" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("src/shaders/masks.vs", "src/shaders/material.fs");
    Shader lightShader("src/shaders/vertices.vs", "src/shaders/light.fs");

    if (!shader.isValid())
    {
        std::cerr << "Vertex or fragment shaders were invalid" << std::endl;
        return -1;
    }

    std::string modelPath = std::filesystem::path("src/models/ellie_animation.obj");
    Model backpack(modelPath);

    Camera camera;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    Light light;

    float deltaTime, lastFrame = 0.0f;

    // // set up lighting
    unsigned int lightVAO, VBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime, camera);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bindTextures();

        shader.use();
        shader.setVec3("light.position", light.position);
        shader.setVec3("viewPos", camera.getPosition());

        // light properties
        glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};
        glm::vec3 diffuseColor = lightColor; // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular", {1.0f, 1.0f, 1.0f});

        // create transformations
        glm::mat4 projection = glm::perspective(camera.getFOV(), (float)SCRN_WIDTH / (float)SCRN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getView();
        shader.setTransform("projection", projection);
        shader.setTransform("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader.setTransform("model", model);

        backpack.draw(shader);

        lightShader.use();
        lightShader.setTransform("projection", projection);
        lightShader.setTransform("view", view);
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, light.position);
        m = glm::scale(m, glm::vec3(0.2f)); // a smaller cube
        lightShader.setTransform("model", m);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime, Camera &camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            camera.resetCamera();
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.rotateCamera(glm::normalize(glm::cross(camera.getFront(), camera.getCameraUp())), 5.0f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.rotateCamera(glm::normalize(glm::cross(camera.getFront(), camera.getCameraUp())), -5.0f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.rotateCamera(camera.getCameraUp(), 5.0f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.rotateCamera(camera.getCameraUp(), -5.0f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.rotateCamera(camera.getFront(), 5.0f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.rotateCamera(camera.getFront(), -5.0f, deltaTime);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveCamera(camera.getFront(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveCamera(-camera.getFront(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveCamera(-glm::normalize(glm::cross(camera.getFront(), camera.getCameraUp())), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveCamera(glm::normalize(glm::cross(camera.getFront(), camera.getCameraUp())), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.moveCamera(camera.getCameraUp(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.moveCamera(-camera.getCameraUp(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.zoomCamera(5.0f, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.zoomCamera(-5.0f, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}