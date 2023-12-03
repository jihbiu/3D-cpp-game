#include "HighlitedCube.h"
#include <iostream>

GLfloat HighlightedCube::s_vertices[] = {
    // Bottom square 
    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,

    // Top square 
    -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,

    // Vertical edges
    -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f 
};

HighlightedCube::HighlightedCube(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

HighlightedCube::~HighlightedCube() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void HighlightedCube::setPosition(const glm::vec3& pos){ m_position = pos; }

void HighlightedCube::draw(Shader& shader) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);

    model = glm::scale(model, glm::vec3(1.2f)); // Make it a bit larger to see it outside the cube
    shader.setMat4("Model", model);
    shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // Red color (RGB)

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}