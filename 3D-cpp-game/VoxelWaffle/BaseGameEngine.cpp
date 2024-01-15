#include "BaseGameEngine.h"

VoxelWaffle::BaseGameEngine::BaseGameEngine()
	: m_window(sf::VideoMode({ 800, 600 }), "Minecraft Clone", sf::Style::Default, sf::ContextSettings{ 24, 0, 1, 3, 3, 0, false }) 
{
	m_window.setActive();
	m_window.setFramerateLimit(60u);
	m_window.setMouseCursorVisible(false);

}
