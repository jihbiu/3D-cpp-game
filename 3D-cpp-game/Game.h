#pragma once
#include "Render/Camera.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include "Render/Shader.h"
#include "Cube.h"

#define GLAP_GL_IMPLEMENTATION

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void init();
    void processEvents(const float& deltaTime);
    void update();
    void render();
    void renderCube();
    std::pair<GLuint, GLuint> createVertexBufferObject();

    sf::Window window;    
    Shader shader;
    Camera camera;
    std::unique_ptr<Cube> cube;

    std::pair<GLuint, GLuint> triangle;
    sf::Clock clock;
    double t = 0.0;

    sf::Vector2i mousePosition;
};


