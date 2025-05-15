#pragma once
#include "ECS.h"
#include "Shader.h"
#include "Mesh.h"

class RenderSystem : public System
{
public:
	RenderSystem(Shader& shader);
	
	/// <summary>
	/// Enables/Disables the debug mode.
	/// This draws models bounding box (AABB) and the world center axis. 
	/// </summary>
	void SetDebugMode(bool status);

	void Update();
private:
	Shader shader;

	bool debugMode = true;

	bool IsValidEntity(Entity& entity);
	void ProcessAnimation(Entity& entity, const std::string& jointName, Matrix4& transform);

	void BindTexture(Mesh* mesh);
	void DrawMesh(Mesh* mesh);
};

