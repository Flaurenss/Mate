#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "ECS.h"
#include "Vector.h"
#include "Camera.h"

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

	static void Test();
	static Engine* instance;
	void testMouse_callback(GLFWwindow* window, double xpos, double ypos);
private:
	Camera* testCamera;
	std::string title;
	float lastFrame;

	float testLastX = 800.0f / 2.0;
	float testLastY = 600.0 / 2.0;

	static int width;
	static int height;
	
	bool isRunning;
	void Run();
	void Initialize();
	void CoreInitialize();
	
	bool testFirstMouse = true;
	GLFWwindow* window;
	std::unique_ptr<ECS> registry;

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void testProcessInput(GLFWwindow* window);
};