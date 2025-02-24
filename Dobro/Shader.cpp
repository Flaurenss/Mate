#include "Shader.h"
#include <filesystem>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        // read file's buffer contents into streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        // TODO: use custom LOGGER
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    // Create and compile VERTEX shader:
    const char* vertexCodeCstr = vertexCode.c_str();
    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexCodeCstr, NULL);
    glCompileShader(vertexShaderId);
    checkShaderErrors(vertexShaderId, "VERTEX");

    // Create and compile FRAGMENT shader:
    const char* fragmentCodeCstr = fragmentCode.c_str();
    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentCodeCstr, NULL);
    glCompileShader(fragmentShaderId);
    checkShaderErrors(fragmentShaderId, "FRAGMENT");

    // Create shader program 
    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderId);
    glAttachShader(ID, fragmentShaderId);
    glLinkProgram(ID);

    checkShaderProgramErrors(ID);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkShaderErrors(unsigned int id, std::string vertexType)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    if (!success)
    {
        std::cout << "ERROR::SHADER::"<< vertexType <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else
    {
        std::cout << "GG::SHADER::" << vertexType << "::COMPILATION_SUCCESSFULL\n" << infoLog << std::endl;
    }
}

void Shader::checkShaderProgramErrors(unsigned int id)
{
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    if (!success)
    {
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else
    {
        std::cout << "GG::SHADER::PROGRAM::COMPILATION_SUCCESSFULL" << infoLog << std::endl;
    }
}
