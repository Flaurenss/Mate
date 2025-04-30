#include "Texture.h"
#include <glad/glad.h>

const Vector4 Texture::DefaultColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

Texture::~Texture()
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}

void Texture::Bind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

unsigned int Texture::GetID() const
{
	return id;
}

bool Texture::IsValid() const
{
	return valid;
}

const std::string& Texture::GetPath() const
{
	return filePath;
}

TextureType Texture::GetType() const
{
	return type;
}

void Texture::SetType(TextureType newType)
{
	type = newType;
}

std::string Texture::TextureTypeToUniformName(TextureType textureType)
{
	auto it = uniformNameMap.find(textureType);
	return (it != uniformNameMap.end()) ? it->second : "";
}

const std::unordered_map<TextureType, std::string> Texture::uniformNameMap = {
	{ TextureType::Diffuse,   "diffuse" },
	{ TextureType::Specular,  "specular" },
};