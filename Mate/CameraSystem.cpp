#include "CameraSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Logger.h"

#include <glad/glad.h>
#include "stb_image.h"

CameraSystem::CameraSystem(Shader& shader, Shader& skyboxShader) :
    shader(shader),
    skyboxShader(skyboxShader),
    skyboxTextureId(0)
{
	RequireComponent<TransformComponent>();
	RequireComponent<CameraComponent>();

    this->skyboxShader.Use();
    skyboxShader.SetInt("skybox", 0);
    glGenTextures(1, &skyboxTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);

    // Table of cube face positions
    //GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
    //GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
    //GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
    //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
    //GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
    //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    int width, height, nrChannels;
    std::vector<std::string> texturePaths =
    { 
        "./Assets/Skybox/right.jpg",
        "./Assets/Skybox/left.jpg",
        "./Assets/Skybox/top.jpg",
        "./Assets/Skybox/bottom.jpg",
        "./Assets/Skybox/front.jpg",
        "./Assets/Skybox/back.jpg"
    };
    for (unsigned int i = 0; i < texturePaths.size(); i++)
    {
        unsigned char* data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << texturePaths[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CameraSystem::Update(RenderContext& renderContext)
{
    for (Entity& entity : GetEntities())
    {
        shader.Use();

        TransformComponent& cameraTransform = entity.GetComponent<TransformComponent>();
        CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();

        Matrix4 projection;
        projection.perspective(
            cameraComponent.Fov,
            static_cast<float>(renderContext.Width) / static_cast<float>(renderContext.Height),
            cameraComponent.Near,
            cameraComponent.Far
        );

        Vector3 position = cameraTransform.Position;
        Vector3 forward = cameraTransform.GetForward().normalize();
        Vector3 right = Vector3::cross(forward, Vector3::Up).normalize();
        Vector3 up = Vector3::cross(right, forward).normalize();

        Matrix4 view = Matrix4::lookAt(position, position + forward, up);

        renderContext.View = view;
        renderContext.Projection = projection;

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Use();
        //view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.SetMat4("view", view);
        skyboxShader.SetMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }
}
