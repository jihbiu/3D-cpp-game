#pragma once

#include "../../../Render/Shader.h"
#include "Cube.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class HighlightedCube {
public:
    HighlightedCube();
    ~HighlightedCube();

    void setPosition(const glm::vec3& pos);

    void draw(Shader& shader);

private:
    static GLfloat s_vertices[];
    glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };

    GLuint vao{ 0 };
    GLuint vbo{ 0 };
};