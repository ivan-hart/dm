#include "resource_manager.h"

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

Shader ResourceManager::LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath)
{
    Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath);
    return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
    return Shaders[name];
}

Texture ResourceManager::LoadTexture(const std::string& name, const char* filePath)
{
    Textures[name] = loadTextureFromFile(filePath);
    return Textures[name];
}

Texture ResourceManager::GetTexture(const std::string& name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
    
    Shaders.clear();
    Textures.clear();
}

ResourceManager::~ResourceManager()
{
    Clear();
}

Shader ResourceManager::loadShaderFromFile(const char* vertexPath, const char* fragmentPath)
{
    Shader shader;
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* filePath)
{
    Texture texture;
    SDL_Surface* surface = IMG_Load(filePath);
    if (surface == nullptr)
    {
        std::cout << "Failed to load texture: " << filePath << std::endl;
        std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
        return texture;
    }

    GLenum format;
    if (surface->format->BytesPerPixel == 4)
    {
        if (surface->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
            format = GL_BGRA;
    }
    else if (surface->format->BytesPerPixel == 3)
    {
        if (surface->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    }
    else
    {
        std::cout << "Warning: Image is not truecolor: " << filePath << std::endl;
        SDL_FreeSurface(surface);
        return texture;
    }

    glGenTextures(1, &texture.ID);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture.Width = surface->w;
    texture.Height = surface->h;
    texture.Format = format;

    SDL_FreeSurface(surface);
    return texture;
}

// Shader class method implementations
void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetInteger(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value)
{
    glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value)
{
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const char* name, const glm::vec4& value)
{
    glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
