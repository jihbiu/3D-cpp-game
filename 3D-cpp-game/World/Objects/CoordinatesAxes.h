#include <SFML/Graphics.hpp>
#include <glad/glad.h>

class CoordinateAxes {
public:
    CoordinateAxes();
    void draw();

private:
    GLuint vbo{ 0 };
    GLuint vao{ 0 };
};