#pragma once

#include "Render/Shader.h"
#include "Render/Camera.h"
#include "World/Objects/Cube.h"
#include "util/PerlinNoise.h"
#include "World/Objects/CoordinatesAxes.h"
#include "World/Chunks/Chunk.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>

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

    sf::Window window;    
    Shader shader;
    Camera camera;

    std::unique_ptr<Cube> cube;
    std::unique_ptr<CoordinateAxes> coordinateAxes;
    std::unique_ptr<PerlinNoise> m_perlinNoise;

    std::unique_ptr<CubePalette> cubePalette;

    std::unique_ptr<Chunk<16, 16, 16>> chunk;

    sf::Clock clock;
    double t = 0.0;

    sf::Vector2i mousePosition;
};


