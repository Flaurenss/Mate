#include "Engine.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

Engine::Engine(int width, int height) :
	widht(width),
	height(height)
{
	title = "Engine";
	ecs = new ECS();
}

Engine::~Engine()
{
}

void Engine::Initialize()
{
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(widht, height, title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW widnow" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	isRunning = true;
}

void Engine::Run()
{
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Engine::ProcessInput()
{
}

void Engine::Update()
{
}

void Engine::Render()
{
}
