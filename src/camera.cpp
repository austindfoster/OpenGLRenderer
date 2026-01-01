#include <camera.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

const glm::mat4 CAMERA_POS({1.0f, 0.0f, 0.0f, 0.0f,   // Column 0
                            0.0f, 1.0f, 0.0f, 0.0f,   // Column 1
                            0.0f, 0.0f, 1.0f, 1.0f,   // Column 2
                            0.0f, 0.0f, 3.0f, 1.0f}); // Column 3
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

const float CAMERA_FOV = glm::radians(45.0f);

Camera::Camera() : m_transform(CAMERA_POS),
                   m_front(CAMERA_FRONT),
                   m_cameraUp(CAMERA_UP),
                   m_speed(2.5f),
                   m_rotSpeed(2.5f),
                   m_fov(CAMERA_FOV)
{
}

void Camera::moveCamera(glm::vec3 direction, float deltaTime)
{
    printf("Direction: (%.2f, %.2f, %.2f)\n", direction[0], direction[1], direction[2]);
    m_transform[3] += glm::vec4(m_speed * deltaTime * direction, 0.0f);
    auto position = m_transform[3];
    printf("Position: (%.2f, %.2f, %.2f)\n", position[0], position[1], position[2]);
}

void Camera::rotateCamera(glm::vec3 axis, float angle, float deltaTime)
{
    angle = glm::radians(angle);
    m_transform = glm::rotate(m_transform, deltaTime * m_rotSpeed * angle, axis);
    m_front = glm::normalize(glm::rotate(m_front, deltaTime * m_rotSpeed * angle, axis));
    m_cameraUp = glm::normalize(glm::rotate(m_cameraUp, deltaTime * m_rotSpeed * angle, axis));
}

void Camera::zoomCamera(float offset, float deltaTime)
{
    m_fov += glm::radians(offset * deltaTime);
}

void Camera::setCameraPosition(glm::vec3 position)
{
    m_transform[3] = glm::vec4(position, 1.0f);
}

void Camera::setCameraRotation(glm::vec3 axis, float angle)
{
    glm::mat4 identity = glm::mat4(1.0f);
    identity[3] = m_transform[3];
    angle = glm::radians(angle);
    m_transform = glm::rotate(identity, angle, axis);
    m_front = glm::normalize(glm::rotate(m_front, angle, axis));
    m_cameraUp = glm::normalize(glm::rotate(m_cameraUp, angle, axis));
}

void Camera::setZoom(float fov)
{
    m_fov = fov;
}

void Camera::resetCamera()
{
    m_transform = CAMERA_POS;
    m_front = CAMERA_FRONT;
    m_cameraUp = CAMERA_UP;
    m_fov = CAMERA_FOV;
}

float Camera::getFOV()
{
    return m_fov;
}

glm::mat4 Camera::getView()
{
    glm::vec3 position = m_transform[3];
    // printf("Position: (%.2f, %.2f, %.2f)\n", position[0], position[1], position[2]);
    // printf("Front: (%.2f, %.2f, %.2f)\n", m_front[0], m_front[1], m_front[2]);
    // printf("Cam Up: (%.2f, %.2f, %.2f)\n", m_cameraUp[0], m_cameraUp[1], m_cameraUp[2]);
    return glm::lookAt(position, position + m_front, m_cameraUp);
}

glm::vec3 Camera::getFront()
{
    return m_front;
}

glm::vec3 Camera::getCameraUp()
{
    return m_cameraUp;
}
