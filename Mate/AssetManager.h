#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Model.h"
#include "Texture.h"
#include "ModelImporter.h"
#include "AudioClip.h"
#include <array>

class AssetManager
{
public:
	static AssetManager& GetInstance();

    void RegisterModel(const std::string& id, std::shared_ptr<Model> model);
    std::shared_ptr<Model> LoadModel(const std::string& id, const std::string& path);
    std::shared_ptr<Model> GetModel(const std::string& id) const;

    std::shared_ptr<Texture> LoadTexture(const std::string& id, const std::string& path);
    std::shared_ptr<Texture> GetTexture(const std::string& id) const;
    std::shared_ptr<Texture> LoadCubemap(const std::string& id, const std::array<std::string, 6>& faces);

    void LoadAudioClip(const std::string& id, const std::string& path);
    AudioClip GetAudioClip(const std::string& id) const;

    void Clear();

private:
    AssetManager() = default;
    ~AssetManager() = default;

    // Defined to dont allow the object copy.
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    ModelImporter modelImporter;
    std::unordered_map<std::string, std::shared_ptr<Model>> models;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, AudioClip> audioClips;
};

