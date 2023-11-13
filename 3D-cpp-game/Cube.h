#pragma once
#include <glad/glad.h>

#include <string>
#include <array>

class Cube {
public:
	Cube(const std::string& texturePath);

	Cube() = delete;
	Cube(const Cube&) = delete;
	Cube& operator=(const Cube&) = delete;
	Cube(Cube&&) noexcept;
	Cube& operator=(Cube&&) noexcept;
	~Cube();

	GLuint Vbo() const { return vbo; }
	GLuint Vao() const { return vao; }
	GLuint Texture() const { return texture; }

private:
	GLuint vbo;
	GLuint vao;
	GLuint texture;

	static std::array<float, 6 * 6 * 5> s_vertices;
};