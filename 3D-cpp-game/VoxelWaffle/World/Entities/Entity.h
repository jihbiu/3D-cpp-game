#pragma once

class Game;

namespace VoxelWaffle
{
	class Entity {
	public:
		Entity(Game& instance);
		virtual ~Entity() = default;

		virtual void update(const float& dt) = 0;

	protected:
		Game& m_instance;
	};
}
