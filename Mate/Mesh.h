#pragma once
#include <Vector.h>
#include <string>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"
#include "Texture.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexureCoordinate;

	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;
	std::string attachedJointName;
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<std::shared_ptr<Texture>> textures);
	~Mesh();

	void SetupMesh();

	unsigned int GetVAO();
	int GetIndexCount();

private:

	// Vertex Array Object: stores pointers to elements from VBO
	unsigned int VAO;
	// Vertex Buffer Object: strores vertex attributes
	unsigned int VBO;
	// Element Buffer Object: stores index data
	unsigned int EBO;

};

