#pragma once

#include "../../util/PerlinNoise.h"
#include "../../Render/Shader.h"
#include "CubePalette.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

template <uint8_t Depth, uint8_t Width, uint8_t Height>
class Chunk {
	struct CubeData {
		Cube::Type m_type{ Cube::Type::None };
		bool m_isVisible{ true };
	};

	using FlattenData_t = std::array<CubeData, Depth* Width* Height>;
public:
	Chunk(const glm::vec2& origin, CubePalette& palette);

	void generate(const PerlinNoise& rng);
	void draw(Shader& shader) const;
private:
	size_t coordsToIndex(size_t depth, size_t width, size_t height) const;
	void updateVisibility();

	CubePalette& m_palette;
	FlattenData_t m_data;
	glm::vec2 m_origin;
};

template<uint8_t Depth, uint8_t Width, uint8_t Height>
Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette)
    : m_palette(palette), m_origin(origin) {

    for (size_t x = 0; x < Width; x++) {
        for (size_t y = 0; y < Height; y++) {
            for (size_t z = 0; z < Depth; z++) {
                auto index = coordsToIndex(z, x, y);
                m_data[index].m_type = Cube::Type::None;
                m_data[index].m_isVisible = true; 
            }
        }
    }
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
void Chunk<Depth, Width, Height>::generate(const PerlinNoise& rng) {

    for (size_t x = 0; x < Width; ++x) {
        for (size_t z = 0; z < Depth; ++z) {
            float scale = 0.15f; 
            float noiseValue = rng.At(glm::vec3((x + m_origin.x) * scale, 0, (z + m_origin.y) * scale));

            size_t terrainHeight = static_cast<size_t>(noiseValue * Height);

            for (size_t y = 0; y < Height; ++y) {
                CubeData& cube = m_data[coordsToIndex(z, x, y)];

                if (y < terrainHeight) {
                    cube.m_type = Cube::Type::Stone;
                }
                else if (y == terrainHeight) {
                    cube.m_type = Cube::Type::Grass;
                }
                else {
                    cube.m_type = Cube::Type::None;
                }
            }
        }
    }
    updateVisibility();
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
void Chunk<Depth, Width, Height>::draw(Shader& shader) const {

    for (size_t x = 0; x < Width; ++x) {
        for (size_t y = 0; y < Height; ++y) {
            for (size_t z = 0; z < Depth; ++z) {
                size_t index = coordsToIndex(z, x, y);
                if (!m_data[index].m_isVisible || m_data[index].m_type == Cube::Type::None) continue;

                const Cube& cube = m_palette.LookUp(m_data[index].m_type);

                glm::vec3 worldPosition(x + m_origin.x, y, z + m_origin.y);

                // Set up model matrix
                glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPosition);
                shader.setMat4("Model", model);

                // Bind the cube's VAO
                glBindVertexArray(cube.Vao());

                glBindTexture(GL_TEXTURE_2D, cube.Texture());
                glDrawArrays(GL_TRIANGLES, 0, 36);

                glBindVertexArray(0);
            }
        }
    }
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline size_t Chunk<Depth, Width, Height>::coordsToIndex(size_t depth, size_t width, size_t height) const {
    return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
        + width * static_cast<size_t>(Depth)
        + depth;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::updateVisibility() {
    //* @TODO
    //  Iterate trough all blocks
    //  Check for neighbour block and if there are not Type:None make the block invisible
    //  Check if the block is on the border of a chunk, if it is make it visible
}
