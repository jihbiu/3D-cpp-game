#pragma once
#include <array>
#include <glm/ext/vector_float3.hpp>

namespace VoxelWaffle {
	class PerlinNoise {
	public:
		PerlinNoise();
		PerlinNoise(float seed);

		float At(const glm::vec3& coords) const;

	private:
		std::array<uint8_t, 256 * 2> m_permutations;
	};
}