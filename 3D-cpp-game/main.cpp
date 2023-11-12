#include <random>

#include <SFML/Graphics.hpp>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp> 
#include "Render/Shader.h"
#include "Render/Camera.h"
#include <iostream>

#define GLAP_GL_IMPLEMENTATION

void processEvents(sf::Window& window, Camera& camera, float deltaTime) {
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

    }
}

std::pair<GLuint, GLuint> CreateVertexBufferObject() {
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



int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion= 3;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL works! :)");
    window.setActive(true);

    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    Shader shader = Shader("res/shaders/vertexShader.shader", "res/shaders/fragmentShader.shader");

    std::pair<GLuint, GLuint> triangle = CreateVertexBufferObject();

    glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Position the camera 3 units away on the Z-axis
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // The point where the triangle is (assuming it's at the origin)
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f); // Up direction of the camera
    glm::vec3 front(0,0,0);

    Camera camera(initialPosition, front, 0.f, 0.f);

    sf::Clock clock;
    double t = 0.0;
    const double dt = 1.0 / 60.0;
    double currentTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();


        double newTime = clock.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        while (frameTime > 0.0) {
            float deltaTime = std::min(frameTime, dt);
            
            processEvents(window, camera, deltaTime);

            frameTime -= deltaTime;
            t += deltaTime;
        }


        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        glm::mat4 view = camera.getView();
        glm::mat4 projection = camera.getProjection();

        shader.setMat4("mvp", glm::mat4(projection * view * model));

        glBindVertexArray(triangle.second);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.display();
    }

    return 0;
}


