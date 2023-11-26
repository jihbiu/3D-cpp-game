#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

// World up vector (static definition)
glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
float Camera::speed = 5.0;

Camera::Camera(
    const glm::vec3& position,
    const glm::vec3& front,
    float yaw, float pitch
) : position(position), 
    front(front), 
    yaw(yaw), 
    pitch(pitch) 
{
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    RecreateLookAt();


    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

Camera Camera::create(const glm::vec3& position, const glm::vec3& front, float yaw, float pitch)
{
    return Camera(position, front, yaw, pitch);
}

void Camera::rotate(const sf::Vector2i& mouseDelta)
{
    float sensitivity = 0.1f;
    yaw += mouseDelta.x * sensitivity;
    pitch += mouseDelta.y * sensitivity;

    pitch = std::max(std::min(pitch, 89.0f), -89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));

    // Update the View Matrix
    RecreateLookAt();
}

void Camera::moveForward(float dt) {
    float velocity = speed * dt;
    position += front * velocity; 

    RecreateLookAt();
}

void Camera::moveBackward(float dt) {
    float velocity = speed * dt;
    position -= front * velocity; 

    RecreateLookAt();
}

void Camera::moveLeft(float dt)
{
    float velocity = speed * dt;
    position -= right * velocity; 

    RecreateLookAt(); 
}

void Camera::moveRight(float dt)
{
    float velocity = speed * dt;
    position += right * velocity; 

    RecreateLookAt();
}

void Camera::moveUp(float dt)
{
    float velocity = speed * dt;
    position += up * velocity;

    RecreateLookAt();
}

void Camera::moveDown(float dt)
{
    float velocity = speed * dt;
    position -= up * velocity;

    RecreateLookAt();
}

void Camera::RecreateLookAt() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    lookAt = glm::lookAt(position, position + front, up);
}
