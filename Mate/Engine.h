#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "ECS.h"

class Engine
{
public:
	Engine(int width = 1920, int height = 1080);
	~Engine();

	ECS& GetECS()
	{
		return *registry;
	}

	bool IsRunning();
	void ProcessInput();
	void Update();
	void Render();

private:
	std::string title;
	int widht;
	int height;
	bool isRunning;
	void Run();
	void Initialize();
	void CoreInitialize();
	GLFWwindow* window;
	std::unique_ptr<ECS> registry;
};