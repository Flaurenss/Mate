#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    /// <summary>
    /// Reads and builds the shader.
    /// </summary>
    /// <param name="vertexPath">The vertex shader path.</param>
    /// <param name="fragmentPath">The fragment shader path.</param>
    Shader(const char* vertexPath, const char* fragmentPath);
    
    /// <summary>
    /// Use/activate the shader.
    /// </summary>
    void Use();

    /// <summary>
    /// Set boolean uniform.
    /// </summary>
    /// <param name="name">The uniform name.</param>
    /// <param name="value">The value.</param>
    void SetBool(const std::string& name, bool value) const;
    
    /// <summary>
    /// Set integer uniform.
    /// </summary>
    /// <param name="name">The uniform name.</param>
    /// <param name="value">The value.</param>
    void SetInt(const std::string& name, int value) const;

    /// <summary>
    /// Set float uniform.
    /// </summary>
    /// <param name="name">The uniform name.</param>
    /// <param name="value">The value.</param>
    void SetFloat(const std::string& name, float value) const;
private:
    void checkShaderErrors(unsigned int id, std::string vertexType);
    void checkShaderProgramErrors(unsigned int id);
};

#endif
