#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>

#include "../util/FileUtil.h"

#include <string>
#include <fstream>


class Shader
{
public:
	Shader() = default;

	Shader(std::string vertrexPath, std::string fragmentPath);
	Shader(GLchar vertexShader[], GLchar fragmentShader[]);

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	~Shader();

	void use();
	GLuint getId();

	void setInt(const std::string_view &name, int value);
	void setMat4(const std::string_view name, const glm::mat4& value);

private:
	GLuint programId;
	
	GLuint createShader(const char* shaderSource, GLenum shaderType);
	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
};

