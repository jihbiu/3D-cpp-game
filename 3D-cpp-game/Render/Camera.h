#pragma once

#include <SFML/Window.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

class Camera {
public:
	Camera(
		const glm::vec3& position,
		const glm::vec3& front,
		float yaw, float pitch
	);

	Camera create(const glm::vec3& position,
		const glm::vec3& front,
		float yaw, float pitch);

	glm::mat4 getView() const { return lookAt; }
	glm::mat4 getProjection() const { return projection; }
	glm::vec3 getPosition() const { return position; }

	void rotate(const sf::Vector2i& mouseDelta);
	void moveForward(double dt);
	void moveBackward(double dt);
	void moveLeft(double dt);
	void moveRight(double dt);
	void moveUp(double dt);
	void moveDown(double dt);

private:
	void RecreateLookAt();

	glm::mat4 projection;
	glm::mat4 lookAt;

	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	float yaw;
	float pitch;

	static glm::vec3 worldUp;
	static float speed;
};