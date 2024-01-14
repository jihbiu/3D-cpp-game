#pragma once
#include "../../../Render/Renderer.h"
#include <glad/glad.h>

#include <string>
#include <array>

namespace VoxelWaffle{
	class Cube {
	public:
		enum class Type {
			None,
			Grass,
			Stone
		};
		Cube(const std::string& texturePath);

		Cube() = delete;
		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;
		Cube(Cube&&) noexcept;
		Cube& operator=(Cube&&) noexcept;
		~Cube();

		void draw() {
			glBindVertexArray(vao);

			glBindTexture(GL_TEXTURE_2D, texture);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		GLuint Vbo() const { return vbo; }
		GLuint Vao() const { return vao; }
		GLuint Texture() const { return texture; }

	private:
		GLuint vbo{ 0 };
		GLuint vao{ 0 };
		GLuint texture{ 0 };

		static std::array<float, 6 * 6 * 5> s_vertices;
	};
}