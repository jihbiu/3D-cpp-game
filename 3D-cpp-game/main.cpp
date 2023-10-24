#include <SFML/Graphics.hpp>
#include <glad/glad.h>

#define GLAP_GL_IMPLEMENTATION


GLchar vertexShader[] =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"      gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\n\0";

GLchar fragmentShader[] =
"#version 330 core\n"
"out vec4 fragmentColor;\n"
"void main(){\n"
"    fragmentColor = vec4(1.f, 0.5, 0.2, 1.f);\n"
"}\n\0";



GLuint CreateShader(const GLchar* shaderSource, GLenum shaderType) {
    const GLuint shaderId = glCreateShader(shaderType);
    if (!shaderId) {
        return 0;   //null handle
    }

    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    // error handeling

    return shaderId;
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

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0)
{
    const GLuint programId = glCreateProgram();
    if (!programId) {
        return 0;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    if (geometryShader) glAttachShader(programId, geometryShader);

    glLinkProgram(programId);

    return programId;
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


    GLuint vShaderId = CreateShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fShaderId = CreateShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLuint programId = CreateProgram(vShaderId, fShaderId);
    std::pair<GLuint, GLuint> triangle = CreateVertexBufferObject();


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))

                window.close();
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programId);
        glBindVertexArray(triangle.first);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.display();
    }

    return 0;
}