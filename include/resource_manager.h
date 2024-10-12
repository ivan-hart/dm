#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glad/glad.h>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
    GLuint ID;
    Shader() : ID(0) {}
    void Use();
    void SetFloat(const char* name, float value);
    void SetInteger(const char* name, int value);
    void SetVector2f(const char* name, const glm::vec2& value);
    void SetVector3f(const char* name, const glm::vec3& value);
    void SetVector4f(const char* name, const glm::vec4& value);
    void SetMatrix4(const char* name, const glm::mat4& matrix);
};

class Texture
{
public:
    GLuint ID;
    int Width, Height;
    GLenum Format;
    Texture() : ID(0), Width(0), Height(0), Format(GL_RGB) {}
};

class ResourceManager
{
public:
    static ResourceManager& GetInstance();

    // Shader methods
    Shader LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath);
    Shader GetShader(const std::string& name);

    // Texture methods
    Texture LoadTexture(const std::string& name, const char* filePath);
    Texture GetTexture(const std::string& name);

    // Clear all loaded resources
    void Clear();

private:
    ResourceManager() {}
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Load and generate a shader from file
    Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath);
    // Load a texture from file
    Texture loadTextureFromFile(const char* filePath);

    // Resource storage
    std::map<std::string, Shader> Shaders;
    std::map<std::string, Texture> Textures;
};

#endif