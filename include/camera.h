#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    void moveCamera(glm::vec3 direction, float deltaTime);
    void rotateCamera(glm::vec3 axis, float angle, float deltaTime);
    void zoomCamera(float offset, float deltaTime);

    void setCameraPosition(glm::vec3 position);
    void setCameraRotation(glm::vec3 axis, float angle);
    void setZoom(float fov);

    void resetCamera();

    float getFOV();
    glm::mat4 getView();
    glm::vec3 getFront();
    glm::vec3 getCameraUp();


private:
    glm::mat4 m_start;
    glm::mat4 m_transform;
    glm::vec3 m_front;
    glm::vec3 m_cameraUp;
    float m_speed;
    float m_rotSpeed;
    float m_fov;
};