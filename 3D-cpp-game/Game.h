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

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void init();
    void processEvents(const double& deltaTime);
    void update();
    void render();
    void renderCube();
    std::pair<GLuint, GLuint> createVertexBufferObject();

    sf::Window m_window;
    Shader m_shader;
    Shader m_HighlitedCubeShader;
    Camera m_camera;

    std::unique_ptr<HighlightedCube> m_highlightedCube;

    std::unique_ptr<PerlinNoise> m_perlinNoise;

    std::unique_ptr<CubePalette> m_cubePalette;

    std::unique_ptr<Chunk<16, 16, 16>> m_chunk;

    sf::Clock m_clock;
    double t = 0.0;

    sf::Vector2i mousePosition;
};


