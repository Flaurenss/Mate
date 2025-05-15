#include "RenderSystem.h"
#include "TransformComponent.h"
#include "EnableComponent.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "DebugDraw.h"
#include "AnimationComponent.h"

RenderSystem::RenderSystem(Shader& sh) : shader(sh)
{
	RequireComponent<TransformComponent>();
	RequireComponent<MeshComponent>();
}

void RenderSystem::SetDebugMode(bool status)
{
	debugMode = status;
}

void RenderSystem::Update()
{
	for (Entity& entity : GetEntities())
	{	
		if (IsValidEntity(entity))
		{
			MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
			Matrix4 center;
			center.translate(-meshComponent.GetCenter());
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			Matrix4 baseTransform = transformComponent.GetTransform();
			const bool isAnimated = entity.HasComponent<AnimationComponent>();

			auto model = AssetManager::GetInstance().GetModel(meshComponent.GetModelId());
			for (const auto& mesh : model->GetMeshes())
			{
				shader.Use();
				// Put model in the center based in its own bounding box (vertices based)
				Matrix4 finalModelTransform = baseTransform * center;

				if (isAnimated)
				{
					ProcessAnimation(entity, mesh->attachedJointName, finalModelTransform);
				}

				shader.SetMat4("model", finalModelTransform);
				BindTexture(mesh.get());
				DrawMesh(mesh.get());
			}

			// ========= DEBUG =========
			// TODO: Add option to enable render
			if (debugMode)
			{
				//Matrix4 transform = transformComponent.GetTransform();
				DebugDraw::DrawAABB(meshComponent.GetExtents() / 2.0f, baseTransform, shader);
				DebugDraw::DrawWorldAxes(shader);
			}
		}
	}
}

bool RenderSystem::IsValidEntity(Entity& entity)
{
	bool isInvisible = entity.HasComponent<EnableComponent>() &&
		!entity.GetComponent<EnableComponent>().Enabled;
	MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
	auto model = AssetManager::GetInstance().GetModel(meshComponent.GetModelId());

	return model && !isInvisible;
}

void RenderSystem::ProcessAnimation(Entity& entity, const std::string& jointName, Matrix4& transform)
{
	auto& animComp = entity.GetComponent<AnimationComponent>();
	auto& jointMap = animComp.GetCache();
	auto iterator = jointMap.find(jointName);
	if (iterator != jointMap.end())
	{
		transform = transform * iterator->second;
	}
}

void RenderSystem::BindTexture(Mesh* mesh)
{
	std::unordered_map<TextureType, int> textureTypeCounters;
	bool validTexture = false;
	int textureUnit = 0;

	for (const auto& texture : mesh->textures)
	{
		if (!texture || !texture->IsValid())
		{
			continue;
		}

		validTexture = true;
		TextureType type = texture->GetType();
		std::string baseName = Texture::TextureTypeToUniformName(type);
		int index = ++textureTypeCounters[type]; // Start at 1
		std::string uniformName = baseName + std::to_string(index);

		// Set uniform to tell shader which texture unit to use
		shader.SetInt(uniformName.c_str(), textureUnit);

		// Bind texture to that unit
		texture->Bind(textureUnit);

		textureUnit++;
	}

	if (validTexture)
	{
		shader.SetBool("valid", true);
	}
	else
	{
		shader.SetVec4("defaultColor", Texture::DefaultColor);
	}
}

void RenderSystem::DrawMesh(Mesh* mesh)
{
	glBindVertexArray(mesh->GetVAO());
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Reset
	glActiveTexture(GL_TEXTURE0);
}
