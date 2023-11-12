#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>


#include "../util/FileUtil.h"

class Shader
{
public:
	Shader(std::string vertrexPath, std::string fragmentPath);
	Shader(GLchar vertexShader[], GLchar fragmentShader[]);

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	~Shader();

	void use();
	GLuint getId();
private:
	GLuint programId;
	
	GLuint createShader(const char* shaderSource, GLenum shaderType);
	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
};

