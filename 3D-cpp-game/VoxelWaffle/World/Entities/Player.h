#pragma once

#include "Entity.h"
#include "../World.h"
#include "../../Render/Camera.h"

namespace VoxelWaffle
{
	class Player : public Entity, public IStreamingSource
	{
	public:
		Player(Game& instance, Camera& camera);
		Player(Game& instance, Camera& camera, glm::vec3 position);
		~Player();

		void update(const float& dt) override;
		void processEvents(const float& dt, const sf::Event& event);

		void placeBlock();
		void removeBlock();

		void setPosition(const glm::vec3& position);
		glm::vec3 getPosition() const override;

	private:
		bool m_leftButtonHeld{ false };
		bool m_rightButtonHeld{ false };
		sf::Vector2i m_mousePosition;

		Camera* m_camera;
		glm::vec3 m_position;
	};
}

