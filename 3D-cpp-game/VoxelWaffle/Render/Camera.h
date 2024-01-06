#pragma once

#include "../World/Collision/Ray.h"

#include <SFML/Window.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
namespace VoxelWaffle {
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

		Ray generateRay();

		void rotate(const sf::Vector2i& mouseDelta);
		void moveForward(float dt);
		void moveBackward(float dt);
		void moveLeft(float dt);
		void moveRight(float dt);
		void moveUp(float dt);
		void moveDown(float dt);

	private:
		void RecreateLookAt();
		float speed{ 10.f };

		glm::mat4 projection;
		glm::mat4 lookAt;

		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 front;
		glm::vec3 right;
		float yaw;
		float pitch;

		static glm::vec3 worldUp;
	};
}