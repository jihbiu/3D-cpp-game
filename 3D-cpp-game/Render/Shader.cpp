#include "Shader.h"
#include <algorithm>


Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    std::string vertexCode = FileUtil::readFile(vertexPath).c_str();
    std::string fragmentCode = FileUtil::readFile(fragmentPath).c_str();

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vShaderId = createShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fShaderId = createShader(fragmentSource, GL_FRAGMENT_SHADER);

    programId = createProgram(vShaderId, fShaderId);

    glDeleteShader(vShaderId);
    glDeleteShader(fShaderId);
}

Shader::Shader(GLchar vertexShader[], GLchar fragmentShader[])
{
    GLuint vShaderId = createShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fShaderId = createShader(fragmentShader, GL_FRAGMENT_SHADER);

    programId = createProgram(vShaderId, fShaderId);
}

Shader::~Shader()
{
    glDeleteProgram(programId);
}

Shader::Shader(Shader&& rhs) noexcept
    : programId(std::exchange(rhs.programId, 0)){
}

Shader& Shader::operator=(Shader&& rhs) noexcept
{
    if (this != &rhs) {
        glDeleteProgram(programId); 

        programId = std::exchange(rhs.programId, 0);
    }
    return *this;
}


void Shader::use()
{
    glUseProgram(programId);
}

GLuint Shader::getId()
{
    return programId;
}


GLuint Shader::createShader(const char* shaderSource, GLenum shaderType)
{
    const GLuint shaderId = glCreateShader(shaderType);
    if (!shaderId) {
        return 0;   //null handle
    }

    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    // Error handeling

    return shaderId;
}

GLuint Shader::createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
    programId = glCreateProgram();
    if (!programId) {
        return 0;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    if (geometryShader) glAttachShader(programId, geometryShader);

    glLinkProgram(programId);

    // Error handeling

    return programId;
}


