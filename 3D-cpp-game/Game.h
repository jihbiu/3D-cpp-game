#pragma once
#include "VoxelWaffle/BaseGameEngine.h"

#include "VoxelWaffle/World/World.h"
#include "VoxelWaffle/Render/Camera.h"
#include "VoxelWaffle/Render/Renderer.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include "VoxelWaffle/World/Entities/Player.h"


#define GLAP_GL_IMPLEMENTATION

class Game : public VoxelWaffle::BaseGameEngine {
public:
    Game();
    ~Game();

    void run() override;

    sf::RenderWindow* getWindow() { return &m_window; };

    VoxelWaffle::IRenderer* getRenderer();
    VoxelWaffle::IWorld* getWorld();

private:
    void processEvents(const float& dt);
    void update(const float& dt);

    std::pair<GLuint, GLuint> createVertexBufferObject();

    VoxelWaffle::Camera m_camera;
    VoxelWaffle::Renderer m_renderer;
    VoxelWaffle::World m_world;
    VoxelWaffle::Player m_player;

    sf::Clock m_clock;
    const double timePerFrame { 1.0 / 60.0 };
    double dt;

    sf::Vector2i mousePosition;
};



