#include "Game.h"

Game::Game()
    : BaseGameEngine()
    , m_camera(glm::vec3(0, 17, 1), glm::vec3(0, 0, 0), 0.f, 0.f)
    , m_renderer(m_window)
    , m_world(*this, 3)
    , m_player(*this, m_camera)
{
    m_renderer.registerCamera(&m_camera);
    m_renderer.registerDrawable(&m_world);
}

Game::~Game() {}

VoxelWaffle::IRenderer* Game::getRenderer() {
    return &m_renderer;
}
VoxelWaffle::IWorld* Game::getWorld() {
	return &m_world;
}

void Game::run()
{
    double t = 0.0;
    double currentTime = m_clock.getElapsedTime().asSeconds();

    while (m_window.isOpen())
    {
        double newTime = m_clock.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        while (frameTime > 0.0) {
            dt = std::min(frameTime, timePerFrame);
			frameTime -= dt;

            processEvents(dt);
            update(dt);
        }

        t += frameTime;

        m_renderer.render();
        m_window.display();
    }
}


void Game::processEvents(const float& dt)
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            m_window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            VoxelWaffle::Ray ray = m_camera.generateRay();
            m_player.processEvents(dt, event);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_camera.moveForward(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_camera.moveBackward(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_camera.moveLeft(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_camera.moveRight(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            m_camera.moveUp(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            m_camera.moveDown(dt);
        }
    }
}

void Game::update(const float& dt)
{
    m_player.update(dt);

    //VoxelWaffle::Ray ray = m_camera.generateRay();
    //VoxelWaffle::AABB::HitRecord hitRecord;

    //if (m_chunk->Hit(ray, 0.f, 100.f, hitRecord) == VoxelWaffle::Ray::HitType::Hit) {
    //    glm::vec3 hitOrigin = hitRecord.m_point;
    //    m_highlightedCube->setPosition(glm::vec3(hitOrigin.x - 0.5f, hitOrigin.y - 0.5f, hitOrigin.z - 0.5f));
    //}
    //else {
    //    m_highlightedCube->setPosition(ray.getOrigin() + ray.getDirection() * 5.f);
    //}
}