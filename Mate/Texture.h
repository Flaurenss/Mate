#pragma once
#include <string>
#include <Vector.h>
#include <unordered_map>

enum class TextureType
{
    Diffuse,
    Specular,
    //Normal,
    //Metallic,
    //Roughness,
    //AO,
    //Emissive,
    Cubemap,
    Unknown
};

class Texture
{
public:
    Texture() = default;
    ~Texture();

    void Bind(unsigned int unit = 0) const;

    unsigned int GetID() const;
    bool IsValid() const;
    TextureType GetType() const;

    void SetType(TextureType newType);

    static const Vector4 DefaultColor;
    static std::string TextureTypeToUniformName(TextureType textureType);

private:
    // Only AssetManager will crete it.
    friend class AssetManager;

    unsigned int id = 0;
    bool valid = false;
    unsigned int textureTarget;
    TextureType type = TextureType::Unknown;

    static const std::unordered_map<TextureType, std::string> uniformNameMap;
};