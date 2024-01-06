#pragma once

#include "../Entities/Objects/Cube.h"

#include <unordered_map>

namespace VoxelWaffle {
	class CubePalette {
	public:
		CubePalette();

		const Cube& LookUp(Cube::Type type) const;

	private:
		std::unordered_map<Cube::Type, Cube> m_palette;
	};
}