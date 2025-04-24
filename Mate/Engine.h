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

	bool IsRunning();
	void ProcessInput();
	void Update();
	void Render();

	static float DeltaTime;

private:
	std::string title;
	float lastFrame;
	float accumulator = 0.0f;

	static int width;
	static int height;
	
	bool isRunning;
	void Run();
	void Initialize();
	void CoreInitialize();
	
	GLFWwindow* window;
	std::unique_ptr<ECS> registry;

	float frameTimeAccumulator = 0.0f;
	int frameCount = 0;
	void ComputeFps(float deltaTime);

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void testProcessInput(GLFWwindow* window);
};