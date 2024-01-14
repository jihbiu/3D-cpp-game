#pragma once

#include <SFML/Graphics.hpp>

namespace VoxelWaffle {
	class BaseGameEngine {
	public:
		BaseGameEngine();
		virtual ~BaseGameEngine() = default;

		virtual void run() = 0;

	protected:
		sf::RenderWindow m_window;
	};
}