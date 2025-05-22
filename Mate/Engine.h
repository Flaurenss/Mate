#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "ECS.h"
#include "Vector.h"

class Engine
{
public:
	Engine(int width = 1920, int height = 1080);
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
	/// Updates the engine.
	/// </summary>
	/// <remarks>
	/// Updates engine systems like ECS (systems), input and render.
	/// </remarks>
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
	/// Adds a new entity to the scene
	/// </summary>
	/// <returns></returns>
	Entity CreateEntity();

private:
	const float fixedDeltaTime = 1.0f / 60.0f;

	bool runSimulation;
	bool isRunning;
	std::string title;
	static int width;
	static int height;
	
	float lastFrame;

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