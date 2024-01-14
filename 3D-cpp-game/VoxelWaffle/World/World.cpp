#include "World.h"

VoxelWaffle::World::World(Game& instance, size_t renderDistance)
	: m_instance(instance)
	, m_renderDistance(renderDistance)
	, m_noise()
{
	m_palette = std::make_unique<CubePalette>();

	glm::ivec2 originChunkPos = worldCoordsToChunkOrigin(glm::vec3(0.f, 0.f, 0.f));

	// Generate chunks within the render distance
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
	glm::vec3 playerPos = { 1,1,1 }; // m_instance.getPlayerPosition();
	glm::ivec2 playerChunkKey = worldCoordsToChunkOrigin(playerPos);

	// Determine the range of chunks to render based on render distance
	for (int dx = -static_cast<int>(m_renderDistance); dx <= static_cast<int>(m_renderDistance); ++dx) {
		for (int dz = -static_cast<int>(m_renderDistance); dz <= static_cast<int>(m_renderDistance); ++dz) {
			glm::ivec2 chunkKey = playerChunkKey + glm::ivec2(dx, dz);

			auto it = m_chunks.find(chunkKey);
			if (it != m_chunks.end()) {
				it->second.draw(shader); 
			}
		}
	}

	//baseChunk->draw(shader);
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
	VoxelWaffle::IWorld::HitRecord& record) const
{


	return VoxelWaffle::Ray::HitType::Miss;
}

void VoxelWaffle::World::removeCube(const glm::ivec3& coords)
{

}

void VoxelWaffle::World::placeCube(const glm::ivec3& coords, VoxelWaffle::Cube::Type type)
{

}

glm::ivec2 VoxelWaffle::World::worldCoordsToChunkOrigin(const glm::vec3& at) const{
	return glm::ivec2(
		static_cast<int>(std::floor(at.x / 32.0f)),
		static_cast<int>(std::floor(at.z / 32.0f))
	);
}