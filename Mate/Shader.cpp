#include "Shader.h"
#include <filesystem>
#include "Logger.h"

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
        Logger::Err(std::string("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: ") + e.what());
        return;
        //std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
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

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec4(const std::string& name, const Vector4& value) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const Matrix4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, value.get());
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
