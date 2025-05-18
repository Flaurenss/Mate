#include "AssetManager.h"
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include "Logger.h"

AssetManager& AssetManager::GetInstance()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::RegisterModel(const std::string& id, std::shared_ptr<Model> model)
{
    if (models.find(id) == models.end())
    {
        models[id] = model;
    }
}

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& id, const std::string& path)
{
    auto it = models.find(id);
    if (it != models.end())
    {
        return it->second;
    }

    std::shared_ptr<Model> model = modelImporter.Load(path);
    if (!model)
    {
        return nullptr;
    }

    models[id] = model;
    return model;
}

std::shared_ptr<Model> AssetManager::GetModel(const std::string& id) const
{
    auto it = models.find(id);
    return (it != models.end()) ? it->second : nullptr;
}

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& id, const std::string& path)
{
    auto it = textures.find(id);
    if (it != textures.end())
    {
        return it->second;
    }

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        Logger::Err("Failed to load texture: " + path);
        return nullptr;
    }

    GLenum format = (nrComponents == 1) ? GL_RED :
        (nrComponents == 3) ? GL_RGB : GL_RGBA;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->id = textureID;
    texture->filePath = path;
    texture->valid = true;

    textures[id] = texture;
    return texture;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& id) const
{
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

void AssetManager::LoadAudioClip(const std::string& id, const std::string& path)
{
    auto audioClip = AudioClip(id, path);
    audioClips.emplace(id, AudioClip(id, path));
}

AudioClip AssetManager::GetAudioClip(const std::string& id) const
{
    auto it = audioClips.find(id);
    if (it != audioClips.end())
    {
        return it->second;
    }
    else
    {
        Logger::Err("Cannot load Clip " + id + " from Assets.");
        throw std::runtime_error("AudioClip not found");
    }
}

void AssetManager::Clear()
{
    models.clear();
    textures.clear();
}
