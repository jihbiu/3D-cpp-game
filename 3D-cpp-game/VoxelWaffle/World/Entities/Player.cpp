#include "Player.h"
#include "../../../Game.h"

VoxelWaffle::Player::Player(Game& instance, Camera& camera)
	: Entity(instance)
	, m_camera(&camera)
	, m_position(glm::vec3(0.f)) {
	m_instance.getWorld()->registerStreamingSource(this);
}

VoxelWaffle::Player::~Player() {
	// unregister camera
	m_instance.getWorld()->unregisterStreamingSource();
}

void VoxelWaffle::Player::update(const float& dt)
{
	sf::Vector2i currentMousePosition = sf::Mouse::getPosition(*m_instance.getWindow());
	sf::Vector2i centerPosition = sf::Vector2i(
		m_instance.getWindow()->getSize().x / 2,
		m_instance.getWindow()->getSize().y / 2
	);

	sf::Vector2i mouseDelta = currentMousePosition - centerPosition;
	m_camera->rotate(mouseDelta);

	sf::Mouse::setPosition(centerPosition, *m_instance.getWindow());
}

void VoxelWaffle::Player::processEvents(const float& dt, const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		Ray ray = m_camera->generateRay();
		AABB::HitRecord hitRecord;

		if (m_instance.getWorld()->hit(ray, 0.f, 10.f, hitRecord) == Ray::HitType::Hit) {
			glm::vec3 hitOrigin = hitRecord.m_point;
		
			if (event.mouseButton.button == sf::Mouse::Left)
				m_instance.getWorld()->removeCube(hitOrigin);
			else if (event.mouseButton.button == sf::Mouse::Right)
				m_instance.getWorld()->placeCube(hitOrigin, ray.getDirection(), hitRecord.m_axis, Cube::Type::Grass);
		}

		/*
		if (m_chunk->Hit(ray, 0.f, 10.f, hitRecord) == VoxelWaffle::Ray::HitType::Hit) {
			glm::vec3 hitOrigin = hitRecord.m_point;
			if (event.mouseButton.button == sf::Mouse::Left)
				m_chunk->RemoveBlock(hitOrigin.x, hitOrigin.y, hitOrigin.z);

			else if (event.mouseButton.button == sf::Mouse::Right) {
				glm::vec3 placePosition = hitRecord.m_point;

				if (hitRecord.m_axis == VoxelWaffle::AABB::Axis::x)
					placePosition.x += (ray.getDirection().x > 0) ? -1 : 1;
				else if (hitRecord.m_axis == VoxelWaffle::AABB::Axis::y)
					placePosition.y += (ray.getDirection().y > 0) ? -1 : 1;
				else if (hitRecord.m_axis == VoxelWaffle::AABB::Axis::z)
					placePosition.z += (ray.getDirection().z > 0) ? -1 : 1;

				m_chunk->PlaceBlock(placePosition.x, placePosition.y, placePosition.z, VoxelWaffle::Cube::Type::Grass);
			}
		}
		*/
	}
}

void VoxelWaffle::Player::setPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 VoxelWaffle::Player::getPosition() const
{
	return m_position;
}
