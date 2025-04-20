#pragma once
#include <Vector.h>
#include <string>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"

struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexureCoordinate;
};

struct Texture {
	unsigned int id;
	// Diffuse or specular
	std::string type;
	bool valid;
	Vector4 defaultColor;
	std::string filePath;
};

const std::string DIFFUSE_NAME = "texture_diffuse";
const std::string SPECULAR_NAME = "texture_specular";
const Vector4 DefaultColor = Vector4(0.502f, 0.502f, 0.502f, 0.502f);

class Mesh : public IRenderable
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	virtual void Draw(Shader& shader);
private:

	// Vertex Array Object: stores pointers to elements from VBO
	unsigned int VAO;
	// Vertex Buffer Object: strores vertex attributes
	unsigned int VBO;
	// Element Buffer Object: stores index data
	unsigned int EBO;

	void SetupMesh();
};

