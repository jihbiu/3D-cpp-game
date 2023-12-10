#include "Game.h"
#include <iostream>

Game::Game()
    :m_camera(
        glm::vec3(5, 10, 5),
        glm::vec3(0, 0, 0),
        0.f, 0.f
    )
{
    init();
}

Game::~Game() {}

void Game::init() {
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion = 3;

    m_window.create(sf::VideoMode(1920, 1080), "Minecraft Clone", sf::Style::Default, contextSettings);
    m_window.setActive(true);

    gladLoadGL();

    glViewport(0, 0, static_cast<GLsizei>(m_window.getSize().x), static_cast<GLsizei>(m_window.getSize().y));

    glEnable(GL_DEPTH_TEST);

    m_shader = Shader("res/shaders/vertexShader.shader", "res/shaders/fragmentShader.shader");

    m_HighlitedCubeShader = Shader("res/shaders/vertexColorShader.shader", "res/shaders/fragmentColorShader.shader");
    m_highlightedCube = std::make_unique<HighlightedCube>();
    
    m_cubePalette = std::make_unique<CubePalette>();
    m_chunk = std::make_unique<Chunk<16, 16, 16>>(
        glm::vec2(0, 0),
        *m_cubePalette
    );    
    
    m_perlinNoise = std::make_unique<PerlinNoise>();
    m_chunk->generate(*m_perlinNoise);
    


    mousePosition = sf::Mouse::getPosition();
}

void Game::run() 
{
    const double dt = 0.01;
    double currentTime = m_clock.getElapsedTime().asSeconds();
    double accumulator = 0.0;

    while (m_window.isOpen()) {
        double newTime = m_clock.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        
        accumulator += frameTime;

        // Get the current mouse position relative to the window
        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(m_window);

        // Calculate the center position of the window
        sf::Vector2i centerPosition = sf::Vector2i(
            m_window.getSize().x / 2,
            m_window.getSize().y / 2
        );

        // Calculate the delta (difference) from the center
        sf::Vector2i mouseDelta = currentMousePosition - centerPosition;

        // Rotate the camera based on the delta
        m_camera.rotate(mouseDelta);

        // Re-center the mouse
        sf::Mouse::setPosition(centerPosition, m_window);
      


        while (accumulator >= dt) {
            update();
            accumulator -= dt;
            t += dt;
        }
        processEvents(dt);
        update();
        render();

        std::string frames = "fps: " + std::to_string(1.0 / frameTime);
        m_window.setTitle(frames);
    }
}

void Game::processEvents(const double &deltaTime) 
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            m_window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            Ray ray = m_camera.generateRay();
            AABB::HitRecord hitRecord;

            if (m_chunk->Hit(ray, 0.f, 10.f, hitRecord) == Ray::HitType::Hit) {
                glm::vec3 hitOrigin = hitRecord.m_point;
                if (event.mouseButton.button == sf::Mouse::Left) 
                    m_chunk->RemoveBlock(hitOrigin.x, hitOrigin.y, hitOrigin.z);
                
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    glm::vec3 placePosition = hitRecord.m_point;

                    if (hitRecord.m_axis == AABB::Axis::x) 
                        placePosition.x += (ray.getDirection().x > 0) ? -1 : 1;
                    else if (hitRecord.m_axis == AABB::Axis::y)
                        placePosition.y += (ray.getDirection().y > 0) ? -1 : 1;
                    else if (hitRecord.m_axis == AABB::Axis::z)
                        placePosition.z += (ray.getDirection().z > 0) ? -1 : 1;

                    m_chunk->PlaceBlock(placePosition.x, placePosition.y, placePosition.z, Cube::Type::Grass);
                }
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_camera.moveForward(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_camera.moveBackward(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_camera.moveLeft(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_camera.moveRight(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            m_camera.moveUp(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            m_camera.moveDown(deltaTime);
        }
    }
}

void Game::update() 
{
    Ray ray = m_camera.generateRay();
    AABB::HitRecord hitRecord;

    if (m_chunk->Hit(ray, 0.f, 100.f, hitRecord) == Ray::HitType::Hit) {
        glm::vec3 hitOrigin = hitRecord.m_point;
        m_highlightedCube->setPosition(glm::vec3(hitOrigin.x - 0.5f, hitOrigin.y - 0.5f, hitOrigin.z - 0.5f));
    }
    else {
        m_highlightedCube->setPosition(ray.getOrigin() + ray.getDirection() * 5.f);
    }
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = m_camera.getView();
    glm::mat4 projection = m_camera.getProjection();
   
    m_shader.setMat4("Model", model);
    m_shader.setMat4("View", view);
    m_shader.setMat4("Projection", projection);

    m_chunk->draw(m_shader);

    m_HighlitedCubeShader.use();
    m_HighlitedCubeShader.setMat4("View", view);
    m_HighlitedCubeShader.setMat4("Projection", projection);

    m_highlightedCube->draw(m_HighlitedCubeShader);

    m_window.display();
}