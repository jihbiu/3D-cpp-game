#include <unordered_map>
#include <utility>
#include <glm/glm.hpp>
#include "Chunks/Chunk.h"

namespace VoxelWaffle {
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;
        }
    };

    class World {
    public:
        World();
        ~World();

        void generateWorld();
        void updateVisibleChunks(const glm::vec3& playerPosition);
        void renderVisibleChunks();

    private:
        std::unordered_map<std::pair<int, int>, Chunk<16, 16, 16>*, pair_hash> chunks;
        std::vector<std::pair<int, int>> visibleChunks; // Coordinates of currently visible chunks

        void loadChunk(int x, int z);
        void unloadChunk(int x, int z);
    };
}