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

void RenderSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		bool isInvisible = entity.HasComponent<EnableComponent>() &&
			!entity.GetComponent<EnableComponent>().Enabled;
		MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
		auto model = AssetManager::GetInstance().GetModel(meshComponent.GetModelId());
		
		if (model && !isInvisible)
		{

			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			Matrix4 baseTransform = transformComponent.GetTransform();

			const bool isAnimated = entity.HasComponent<AnimationComponent>();

			Matrix4 center;
			center.translate(-meshComponent.GetCenter());

			for (const auto& mesh : model->GetMeshes())
			{
				shader.Use();
				Matrix4 finalModelTransform = baseTransform * center;

				if (isAnimated)
				{
					auto& animComp = entity.GetComponent<AnimationComponent>();
					auto& jointMap = animComp.GetCache();
					auto it = jointMap.find(mesh->attachedJointName);
					if (it != jointMap.end())
					{
						finalModelTransform = finalModelTransform * it->second;
					}
				}

				shader.SetMat4("model", finalModelTransform);
				BindTexture(mesh.get());
				DrawMesh(mesh.get());
			}

			// ========= DEBUG =========
			Matrix4 transform = transformComponent.GetTransform();
			DebugDraw::DrawAABB(meshComponent.GetExtents() / 2.0f, transform, shader);
			DebugDraw::DrawWorldAxes(shader);
		}
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
