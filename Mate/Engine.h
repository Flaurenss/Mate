#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "ECS.h"
#include "Vector.h"
#include "RenderContext.h"

class Engine
{
public:
	Engine(int width = 1920, int height = 1080, std::string title = "Mate.Engine");
	~Engine();

	/// <summary>
	/// The actual delta time between frames.
	/// </summary>
	static float DeltaTime;

	/// <summary>
	/// Checks if the engine is running.
	/// </summary>
	/// <returns>Engine actual running status.</returns>
	bool IsRunning();
	
	/// <summary>
	/// Updates the engine systems like ECS (systems), input and render.
	/// </summary>
	void Update();

	/// <summary>
	/// Updates the physics simulation.
	/// </summary>
	/// <param name="fixedDeltaTime">The step of time tu simulate.</param>
	void PhysicsUpdate(float fixedDeltaTime);
	
	/// <summary>
	/// Sets simulation to desired status (on/off).
	/// </summary>
	/// <param name="status">The acitve status.</param>
	void SetSimulationTo(bool status);

	/// <summary>
	/// Adds a new entity to the scene.
	/// </summary>
	/// <returns>The entity.</returns>
	Entity CreateEntity();

	/// <summary>
	/// Enable render debug mode.
	/// Enables/Disables the debug mode.
	/// This draws models bounding box (AABB) and the world center axis.
	/// </summary>
	/// <param name="mode">
	/// The boolean status to set.
	/// </param>
	void SetRenderDebugMode(bool mode);

private:
	const float fixedDeltaTime = 1.0f / 60.0f;

	bool runSimulation;
	bool isRunning;
	std::string title;
	float lastFrame;

	RenderContext renderContext;

	void Initialize();
	void CoreInitialize();

	void ComputeDelta();
	void RenderUpdate();

	GLFWwindow* window;
	std::unique_ptr<ECS> registry;

	float frameTimeAccumulator = 0.0f;
	int frameCount = 0;
	void DebugFps(float deltaTime);

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void testProcessInput(GLFWwindow* window);
};