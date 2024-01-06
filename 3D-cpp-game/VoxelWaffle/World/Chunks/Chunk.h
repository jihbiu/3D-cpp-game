#pragma once

#include "../../util/PerlinNoise.h"
#include "../../Render/Shader.h"
#include "../Collision/AABB.h"
#include "../Collision/Ray.h"
#include "CubePalette.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


namespace VoxelWaffle {
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

        Ray::HitType Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, AABB::HitRecord& record) const;

        bool RemoveBlock(uint8_t width, uint8_t height, uint8_t depth);
        bool PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type);
    private:
        size_t coordsToIndex(size_t depth, size_t width, size_t height) const;
        void updateVisibility();

        CubePalette& m_palette;
        FlattenData_t m_data;
        glm::vec2 m_origin;
        AABB m_aabb;
    };

    template<uint8_t Depth, uint8_t Width, uint8_t Height>
    Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette)
        : m_palette(palette)
        , m_origin(origin)
        , m_aabb(
            glm::vec3(origin.x, 0 - Height, origin.y),
            glm::vec3(origin.x + Width, 0, origin.y + Depth)
        ) {
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
        float scale = 0.15f;

        for (size_t x = 0; x < Width; x++) {
            for (size_t z = 0; z < Depth; z++) {
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
    inline Ray::HitType Chunk<Depth, Width, Height>::Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, AABB::HitRecord& record) const
    {
        Ray::time_t hitTimeMax = std::numeric_limits<Ray::time_t>::max();
        size_t hitCubeIndex = -1;

        for (int x = 0; x < Width; x++) {
            for (int y = 0; y < Height; y++) {
                for (int z = 0; z < Depth; z++) {
                    size_t index = coordsToIndex(z, x, y);
                    const CubeData& cube = m_data[index];

                    if (cube.m_type == Cube::Type::None)
                        continue;

                    glm::vec3 cubeAABBMin = glm::vec3(x + m_origin.x, y, z + m_origin.y);
                    glm::vec3 cubeAABBMax = cubeAABBMin + glm::vec3(1, 1, 1);
                    AABB cubeAABB(cubeAABBMin, cubeAABBMax);

                    AABB::HitRecord cubeHitRecord;
                    if (cubeAABB.Hit(ray, min, max, cubeHitRecord) == Ray::HitType::Hit) {
                        if (cubeHitRecord.m_time < hitTimeMax) {
                            hitTimeMax = cubeHitRecord.m_time;
                            hitCubeIndex = index;

                            glm::vec3 cubeCenter = glm::vec3((cubeAABBMin + cubeAABBMax) * 0.5f);

                            record.m_point = cubeCenter;    //ray.at(cubeHitRecord.m_time);
                            record.m_axis = cubeHitRecord.m_axis;
                        }
                    }
                }
            }
        }

        if (hitCubeIndex != -1) {
            record.m_time = hitTimeMax;
            return Ray::HitType::Hit;
        }

        return Ray::HitType::Miss;
    }

    template<uint8_t Depth, uint8_t Width, uint8_t Height>
    inline bool Chunk<Depth, Width, Height>::RemoveBlock(uint8_t width, uint8_t height, uint8_t depth)
    {
        size_t index = coordsToIndex(depth, width, height);
        CubeData& cube = m_data[index];

        if (cube.m_type == Cube::Type::None)
            return false;

        cube.m_type = Cube::Type::None;
        updateVisibility();
        return true;
    }

    template<uint8_t Depth, uint8_t Width, uint8_t Height>
    inline bool Chunk<Depth, Width, Height>::PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type)
    {
        size_t index = coordsToIndex(depth, width, height);
        CubeData& cube = m_data[index];

        if (cube.m_type != Cube::Type::None)
            return false;

        cube.m_type = type;
        updateVisibility();
        return true;
    }


    template<uint8_t Depth, uint8_t Width, uint8_t Height>
    inline size_t Chunk<Depth, Width, Height>::coordsToIndex(size_t depth, size_t width, size_t height) const {
        return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
            + width * static_cast<size_t>(Depth)
            + depth;
    }

    template<uint8_t Depth, uint8_t Width, uint8_t Height>
    inline void Chunk<Depth, Width, Height>::updateVisibility() {
        // @IDEA optimize for only walls visible not whole cube
        for (size_t x = 0; x < Width; ++x) {
            for (size_t y = 0; y < Height; ++y) {
                for (size_t z = 0; z < Depth; ++z) {
                    size_t index = coordsToIndex(z, x, y);
                    CubeData& cube = m_data[index];

                    if (cube.m_type == Cube::Type::None) {
                        cube.m_isVisible = false;
                        continue;
                    }

                    bool onBoundary = x == 0 || x == Width - 1 ||
                        y == 0 || y == Height - 1 ||
                        z == 0 || z == Depth - 1;

                    if (!onBoundary)
                        cube.m_isVisible =
                        m_data[coordsToIndex(z, x - 1, y)].m_type == Cube::Type::None ||
                        m_data[coordsToIndex(z, x + 1, y)].m_type == Cube::Type::None ||
                        m_data[coordsToIndex(z, x, y - 1)].m_type == Cube::Type::None ||
                        m_data[coordsToIndex(z, x, y + 1)].m_type == Cube::Type::None ||
                        m_data[coordsToIndex(z - 1, x, y)].m_type == Cube::Type::None ||
                        m_data[coordsToIndex(z + 1, x, y)].m_type == Cube::Type::None;
                    else
                        cube.m_isVisible = true;
                }
            }
        }
    }
}