#include "World.h"

VoxelWaffle::World::World(Game& instance, size_t renderDistance)
	: m_instance(instance)
	, m_renderDistance(renderDistance)
	, m_noise()
{
	m_palette = std::make_unique<CubePalette>();

	glm::ivec2 originChunkPos = worldCoordsToChunkOrigin(glm::vec3(0.f, 0.f, 0.f));

	// Generate chunks within the render distance
	float tempDistancer = 0.f;
	for (int dx = -static_cast<int>(m_renderDistance); dx <= static_cast<int>(m_renderDistance); dx++){
		for (int dz = -static_cast<int>(m_renderDistance); dz <= static_cast<int>(m_renderDistance); dz++){
			glm::ivec2 chunkPos = originChunkPos + glm::ivec2(dx, dz);
			glm::vec2 worldPos(chunkPos.x * FixedChunkSize::Width, chunkPos.y * FixedChunkSize::Depth);

			glm::ivec2 chunkKey = originChunkPos + glm::ivec2(dx, dz);			
			FixedSizeChunk& chunk = m_chunks.insert({ chunkPos, FixedSizeChunk(worldPos, *m_palette) }).first->second;
			chunk.generate(m_noise);
		}
	}
}

VoxelWaffle::World::~World() 
{
}

void VoxelWaffle::World::update(float deltaTime)
{

}

void VoxelWaffle::World::draw(VoxelWaffle::Shader& shader)
{
	glm::vec3 playerPos = { 0,3,0 }; // m_instance.getPlayerPosition();
	glm::ivec2 playerChunkKey = worldCoordsToChunkOrigin(playerPos);

	// range of chunks to render based on render distance and Player position
	for (int dx = -static_cast<int>(m_renderDistance); dx <= static_cast<int>(m_renderDistance); ++dx) {
		for (int dz = -static_cast<int>(m_renderDistance); dz <= static_cast<int>(m_renderDistance); ++dz) {
			glm::ivec2 chunkKey = playerChunkKey + glm::ivec2(dx, dz);

			auto it = m_chunks.find(chunkKey);
			if (it != m_chunks.end()) {
				it->second.draw(shader); 
			}
		}
	}
}

void VoxelWaffle::World::registerStreamingSource(VoxelWaffle::IStreamingSource* source)
{
	m_streamingSource = source;
}

void VoxelWaffle::World::unregisterStreamingSource()
{
	m_streamingSource = nullptr;
}

VoxelWaffle::Ray::HitType VoxelWaffle::World::hit(
	const VoxelWaffle::Ray& ray,
	VoxelWaffle::Ray::time_t min,
	VoxelWaffle::Ray::time_t max,
	VoxelWaffle::AABB::HitRecord& record) const
{
	Ray::HitType result = Ray::HitType::Miss;
	glm::ivec2 hitChunkPosition;

	//Check for neighbour if not hit aswell based on the direction of the ray;
	hitChunkPosition = worldCoordsToChunkOrigin(ray.getOrigin());
	FixedSizeChunk it = m_chunks.at(hitChunkPosition);

	if (it.Hit(ray, 0.f, 10.f, record) == Ray::HitType::Hit) 
		result = Ray::HitType::Hit;
	
	return result;
}

void VoxelWaffle::World::removeCube(const glm::ivec3& coords)
{
	glm::vec2 hitChunkPosition = worldCoordsToChunkOrigin(coords);
	FixedSizeChunk& it = m_chunks.at(hitChunkPosition);

	it.RemoveBlock(
		coords.x % FixedChunkSize::Width,
		coords.y % FixedChunkSize::Height,
		coords.z % FixedChunkSize::Depth
	);
}

void VoxelWaffle::World::placeCube(const glm::ivec3& coords, const glm::vec3& rayDirection, const AABB::Axis& axis, VoxelWaffle::Cube::Type type)
{
	glm::vec2 hitChunkPosition = worldCoordsToChunkOrigin(coords);
	FixedSizeChunk& it = m_chunks.at(hitChunkPosition);

	//Which site of the block it was placed on
	glm::vec3 blockTranslate = glm::vec3(0.f);

	if (axis == VoxelWaffle::AABB::Axis::x)
		blockTranslate.x = (rayDirection.x > 0) ? -1 : 1;
	else if (axis == VoxelWaffle::AABB::Axis::y)
		blockTranslate.y = (rayDirection.y > 0) ? -1 : 1;
	else if (axis == VoxelWaffle::AABB::Axis::z)
		blockTranslate.z = (rayDirection.z > 0) ? -1 : 1;

	it.PlaceBlock(
		coords.x % FixedChunkSize::Width + blockTranslate.x,
		coords.y % FixedChunkSize::Height + blockTranslate.y,
		coords.z % FixedChunkSize::Depth + blockTranslate.z,
		type
	);
}

glm::ivec2 VoxelWaffle::World::worldCoordsToChunkOrigin(const glm::vec3& at) const{
	return glm::ivec2(
		static_cast<int>(std::floor(at.x / World::FixedChunkSize::Width)),
		static_cast<int>(std::floor(at.z / World::FixedChunkSize::Depth))
	);
}