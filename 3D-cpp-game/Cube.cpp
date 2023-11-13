#include "Cube.h"

#include <SFML/Graphics.hpp>
#include <iostream>

std::array<float, 6 * 6 * 5> Cube::s_vertices = {
	//x       y     z       u     v
	// Back wall
	 -0.5f, -0.5f, -0.5f,  0.25f, 0.0f,
	  0.5f, -0.5f, -0.5f,  0.5f, 0.0f,
	  0.5f,  0.5f, -0.5f,  0.5f, 1.0f / 3.0f,
	  0.5f,  0.5f, -0.5f,  0.5f, 1.0f / 3.0f,
	 -0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
	 -0.5f, -0.5f, -0.5f,  0.25f, 0.0f,


	 // Front Wall
	 -0.5f, -0.5f,  0.5f,  0.25f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.5f, 1.0f,
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
	 -0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,
	 -0.5f, -0.5f,  0.5f,  0.25f, 1.f,


	 //Left Wall
	 -0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,
	 -0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f / 3.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f / 3.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 2.0f / 3.0f,
	 -0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,


	 //Right Wall
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
	  0.5f,  0.5f, -0.5f,  0.5f, 1.0f / 3.0f,
	  0.5f, -0.5f, -0.5f,  0.75f, 1.0f / 3.0f,
	  0.5f, -0.5f, -0.5f,  0.75f, 1.0f / 3.0f,
	  0.5f, -0.5f,  0.5f,  0.75f, 2.0f / 3.0f,
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,

	  //Down wall
	 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f / 3.0f,
	  0.5f, -0.5f, -0.5f,  0.75f, 1.0f / 3.0f,
	  0.5f, -0.5f,  0.5f,  0.75f, 2.0f / 3.0f,
	  0.5f, -0.5f,  0.5f,  0.75f, 2.0f / 3.0f,
	 -0.5f, -0.5f,  0.5f,  1.0f, 2.0f / 3.0f,
	 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f / 3.0f,

	 //Upper wall
	 -0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
	  0.5f,  0.5f, -0.5f,  0.5f, 1.0f / 3.0f,
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
	  0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
	 -0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,
	 -0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f
};

Cube::Cube(const std::string& texturePath) :
	vao(0),
	vbo(0),
	texture(0)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture position attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	sf::Image image;
	if (!image.loadFromFile(texturePath)) {
		std::cerr << "Failed to load texture" << std::endl;
		return;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::Cube(Cube&& rhs) noexcept
	: vbo(std::exchange(rhs.vbo, 0))
	, vao(std::exchange(rhs.vao, 0))
	, texture(std::exchange(rhs.texture, 0)) {
}

Cube& Cube::operator=(Cube&& rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}

	vbo = std::exchange(rhs.vbo, 0);
	vao = std::exchange(rhs.vao, 0);
	texture = std::exchange(rhs.texture, 0);

	return *this;
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteTextures(1, &texture);
}
