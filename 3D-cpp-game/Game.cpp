#include "Game.h"

#include "game.h"
#include <iostream>
#include <algorithm>

Game::Game()
    :camera(glm::vec3(5.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 0.f, 0.f)
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

    window.create(sf::VideoMode(800, 600), "OpenGL works! :)", sf::Style::Default, contextSettings);
    window.setActive(true);

    gladLoadGL();

    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    glEnable(GL_DEPTH_TEST);

    shader = Shader("res/shaders/vertexShader.shader", "res/shaders/fragmentShader.shader");
    triangle = createVertexBufferObject();
    cube = std::make_unique<Cube>("res/textures/grass.jpg");

    mousePosition = sf::Mouse::getPosition();
}

void Game::run() 
{
    const double dt = 0.01;
    double currentTime = clock.getElapsedTime().asSeconds();
    double accumulator = 0.0;

    while (window.isOpen()) {
        double newTime = clock.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        
        accumulator += frameTime;

        const sf::Vector2i newMousePosition = sf::Mouse::getPosition();
        camera.rotate(newMousePosition - mousePosition);
        mousePosition = newMousePosition;

        while (accumulator >= dt) {
            processEvents(dt);
            update();

            accumulator -= dt;
            t += dt;
        }
        render();
    }
}

void Game::processEvents(const float &deltaTime) 
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.moveForward(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.moveBackward(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.moveLeft(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.moveRight(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            camera.moveUp(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            camera.moveDown(deltaTime);
        }
        std::cout << camera.getPosition().x << " | " << camera.getPosition().y << " | " << camera.getPosition().z << "\n";
    }
}

void Game::update() 
{


}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getView(); 
    glm::mat4 projection = camera.getProjection(); 

    shader.setMat4("mvp", glm::mat4(projection * view * model));

    renderCube();

    window.display();
}

void Game::renderCube() {
    glBindVertexArray(cube->Vao());

    glBindTexture(GL_TEXTURE_2D, cube->Texture());
    glDrawArrays(GL_TRIANGLES, 0, 36); 

    glBindVertexArray(0);
}

std::pair<GLuint, GLuint> Game::createVertexBufferObject() {
    const float triangle[] = {
        //  x     y      z
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
    };

    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return std::make_pair(vbo, vao);
}
