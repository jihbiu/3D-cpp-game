#include "World.h"
/*
VoxelWaffle::World::World() {}

VoxelWaffle::World::~World() {
    for (auto& pair : chunks) {
        delete pair.second;
    }
}

void VoxelWaffle::World::generateWorld() {
    for (int x = -5; x <= 5; ++x) {
        for (int z = -5; z <= 5; ++z) {
            loadChunk(x, z);
        }
    }
}

void VoxelWaffle::World::updateVisibleChunks(const glm::vec3& playerPosition) {
    // Update the list of visible chunks based on the player's position
    // This could involve some sort of view frustum culling or distance checks
    // For simplicity, let's just mark all existing chunks as visible
    visibleChunks.clear();
    for (const auto& pair : chunks) {
        visibleChunks.push_back(pair.first);
    }
}

void VoxelWaffle::World::renderVisibleChunks() {
    // Render all chunks that are currently visible
    for (const auto& coords : visibleChunks) {
        
        VoxelWaffle::Chunk* chunk = chunks[coords];
        if (chunk) {
            chunk->render();
        }
    }
}

void VoxelWaffle::World::loadChunk(int x, int z) {
    // Load or generate a chunk at the specified coordinates
    if (chunks.find({ x, z }) == chunks.end()) {
        chunks[{x, z}] = new VoxelWaffle::Chunk();
    }
}

void VoxelWaffle::World::unloadChunk(int x, int z) {
    // Unload and delete a chunk at the specified coordinates
    auto it = chunks.find({ x, z });
    if (it != chunks.end()) {
        delete it->second;
        chunks.erase(it);
    }
}*/
