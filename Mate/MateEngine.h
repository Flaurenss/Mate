#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "ECS.h"
#include "Vector.h"
#include "RenderContext.h"
#include "IGame.h"

class MateEngine
{
public:
	/// <summary>
	/// Engine constructor.
	/// </summary>
	/// <param name="width">The widtht to start the window.</param>
	/// <param name="height">The height to start the window.</param>
	/// <param name="title">The title to appear on the window.</param>
	MateEngine(int width = 1920, int height = 1080, std::string title = "Mate.Engine");
	
	~MateEngine();

	/// <summary>
	/// Enable show FPS logger.
	/// </summary>
	/// <param name="activate">The status to activate.</param>
	void ShowFrames(bool activate = true);

	/// <summary>
	/// Adds a new entity to the scene.
	/// </summary>
	/// <returns>The entity.</returns>
	Entity CreateEntity();

	/// <summary>
	/// Sets skybox.
	/// </summary>
	/// <param name="id">The skybox identifier.</param>
	/// <param name="faces">
	/// The 6 faces to compose the skybox.
	/// With order:
	/// Right->
	/// Left->
	/// Top->
	/// Bottom->
	/// Front->
	/// Back
	/// </param>
	void SetSkybox(const std::string& id, std::array<std::string, 6> faces);

	/// <summary>
	/// Enable render debug mode.
	/// Enables/Disables the debug mode.
	/// This draws models bounding box (AABB) and the world center axis.
	/// </summary>
	/// <param name="mode">
	/// The boolean status to set.
	/// </param>
	void SetRenderDebugMode(bool mode = true);

	/// <summary>
	/// Runs the game.
	/// </summary>
	/// <param name="game">The game instance to run.</param>
	void Run(IGame& game);

private:
	const float fixedDeltaTime = 1.0f / 60.0f;
	float deltaTime;

	std::string title;
	bool isRunning;
	GLFWwindow* window;
	std::unique_ptr<ECS> registry;
	RenderContext renderContext;
	
	bool showFps = false;
	float lastFrame = 0.0f;
	int frameCount = 0;
	float frameTimeAccumulator = 0.0f;

	void Initialize();
	void CoreInitialize();
	void DebugFps(float deltaTime);

	void ComputeDelta();
	void RenderUpdate();
	void PhysicsUpdate(float fixedDeltaTime);

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
};