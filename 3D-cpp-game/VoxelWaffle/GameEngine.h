#pragma once

#include "Render/Shader.h"
#include "Render/Camera.h"
#include "util/PerlinNoise.h"
#include "World/Chunks/Chunk.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>
#include "World/Entities/Objects/HighlitedCube.h"

#define GLAP_GL_IMPLEMENTATION

namespace VoxelWaffle {

    class GameEngine {
    public:
        GameEngine();
        ~GameEngine();

        void run();

    private:
        void init();
        void processEvents(const double& deltaTime);
        void update();
        void render();
        void renderCube();
        std::pair<GLuint, GLuint> createVertexBufferObject();

        sf::Window m_window;
        VoxelWaffle::Shader m_shader;
        VoxelWaffle::Shader m_HighlitedCubeShader;
        VoxelWaffle::Camera m_camera;

        std::unique_ptr<VoxelWaffle::HighlightedCube> m_highlightedCube;
        std::unique_ptr<VoxelWaffle::PerlinNoise> m_perlinNoise;
        std::unique_ptr<VoxelWaffle::CubePalette> m_cubePalette;
        std::unique_ptr<VoxelWaffle::Chunk<16, 16, 16>> m_chunk;

        sf::Clock m_clock;
        const double dt = 0.01;

        sf::Vector2i mousePosition;
    };
}


