#pragma once

#include "Chunks/CubePalette.h"
#include "Chunks/Chunk.h"
#include "Collision/Ray.h"
#include "../util/PerlinNoise.h"
#include "../Render/Renderer.h"

#include <glm/glm.hpp>

#include <unordered_map>

class Game;

namespace VoxelWaffle {
    struct IVecHash {
        std::size_t operator()(const glm::ivec2& vec) const {
            return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
        }
    };

    class IStreamingSource {
    public:
        virtual ~IStreamingSource() = default;
        virtual glm::vec3 getPosition() const = 0;
    };

    class IWorld {
    public:
        struct HitRecord {
            glm::ivec3 m_cubeCoordinates;
            glm::ivec3 m_neighbourCoordinates;
        };

        virtual ~IWorld() = default;

        virtual void registerStreamingSource(IStreamingSource* source) = 0;
        virtual void unregisterStreamingSource() = 0;

        virtual Ray::HitType hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const = 0;
        virtual void removeCube(const glm::ivec3& coords) = 0;
        virtual void placeCube(const glm::ivec3& coords, Cube::Type type) = 0;
    };

    class World : public IWorld, public IDrawable {
    public:
        struct FixedChunkSize {
            static const size_t Width = 8;
            static const size_t Height = 8;
            static const size_t Depth = 8;
        };
        using FixedSizeChunk = Chunk<
            FixedChunkSize::Width,
            FixedChunkSize::Height,
            FixedChunkSize::Depth
        >;

        World(Game& instance, size_t renderDistance);
        ~World();

        void update(float deltaTime);

        void draw(Shader& shader);

        void registerStreamingSource(VoxelWaffle::IStreamingSource* source) override;
        void unregisterStreamingSource() override;

        Ray::HitType hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const override;
        void removeCube(const glm::ivec3& coords) override;
        void placeCube(const glm::ivec3& coords, Cube::Type type) override;

    private:
        glm::ivec2 worldCoordsToChunkOrigin(const glm::vec3& at) const;

        IStreamingSource* m_streamingSource{ nullptr };
        std::unique_ptr<FixedSizeChunk> baseChunk;

        std::unordered_map<glm::ivec2, FixedSizeChunk, IVecHash> m_chunks;
        std::unique_ptr<CubePalette> m_palette;
        PerlinNoise m_noise;

        size_t m_renderDistance;
        Game& m_instance;
    };
}