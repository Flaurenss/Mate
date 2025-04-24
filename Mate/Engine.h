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

	ECS& GetRegistry()
	{
		return *registry;
	}

	static float DeltaTime;

	bool IsRunning();
	void Update();

private:
	const float fixedDeltaTime = 1.0f / 60.0f;

	bool isRunning;
	std::string title;
	static int width;
	static int height;
	
	float lastFrame;
	float accumulator = 0.0f;

	void Initialize();
	void CoreInitialize();

	void ComputeDelta();
	void FixedUpdate();
	void RenderUpdate();

	GLFWwindow* window;
	std::unique_ptr<ECS> registry;

	float frameTimeAccumulator = 0.0f;
	int frameCount = 0;
	void DebugFps(float deltaTime);

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void testProcessInput(GLFWwindow* window);
};