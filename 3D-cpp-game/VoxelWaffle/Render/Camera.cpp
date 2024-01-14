#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

glm::vec3 VoxelWaffle::Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

VoxelWaffle::Camera::Camera(
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

    //RecreateLookAt();

    projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    RecreateLookAt();
}

VoxelWaffle::Camera VoxelWaffle::Camera::create(const glm::vec3& position, const glm::vec3& front, float yaw, float pitch)
{
    return VoxelWaffle::Camera(position, front, yaw, pitch);
}

VoxelWaffle::Ray VoxelWaffle::Camera::generateRay()
{
    glm::vec3 rayOrigin = position + glm::vec3(0.5);//glm::vec3(position.x + 0.5, position.y + 0.5, position.z + 0.5);
    glm::vec3 rayDirection = glm::normalize(front + glm::vec3(0.f, 0.f, 0.f));

    return Ray(rayOrigin, rayDirection);
}

void VoxelWaffle::Camera::rotate(const sf::Vector2i& mouseDelta)
{
    float sensivity = 0.3f;
    yaw += (mouseDelta.x * sensivity);
    pitch -= (mouseDelta.y * sensivity);

    pitch = std::min(std::max(pitch, -89.f), 89.f);
    
    RecreateLookAt();
}
void VoxelWaffle::Camera::moveForward(const float& dt) {
    float velocity = speed * dt;
    position += front * velocity;

    RecreateLookAt();
}
void VoxelWaffle::Camera::moveBackward(const float& dt) {
    float velocity = speed * dt;
    position -= front * velocity; 

    RecreateLookAt();
}
void VoxelWaffle::Camera::moveLeft(const float& dt){
    float velocity = speed * dt;
    position -= right * velocity; 

    RecreateLookAt(); 
}
void VoxelWaffle::Camera::moveRight(const float& dt){
    float velocity = speed * dt;
    position += right * velocity; 

    RecreateLookAt();
}
void VoxelWaffle::Camera::moveUp(const float& dt){
    float velocity = speed * dt;
    position.y += velocity;

    RecreateLookAt();
}
void VoxelWaffle::Camera::moveDown(const float& dt)
{
    float velocity = speed * dt;
    position.y -= velocity;

    RecreateLookAt();
}

void VoxelWaffle::Camera::RecreateLookAt() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    lookAt = glm::lookAt(position, position + front, up);
}
