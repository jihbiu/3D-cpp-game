#include "CubePalette.h"

VoxelWaffle::CubePalette::CubePalette(){
    m_palette.insert({ Cube::Type::Grass, Cube("res/textures/grass.jpg") });
    m_palette.insert({ Cube::Type::Stone, Cube("res/textures/stone.jpg") });
    m_palette.insert({ Cube::Type::None , Cube("none")});
}

const VoxelWaffle::Cube& VoxelWaffle::CubePalette::LookUp(VoxelWaffle::Cube::Type type) const{
	return m_palette.at(type);
}
