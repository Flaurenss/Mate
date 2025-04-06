#include "Engine.h"
#include <iostream>
#include "Logger.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "stb_image.h"
#include "RenderSystem.h"

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);

Engine::Engine(int width, int height) :
	widht(width),
	height(height),
	isRunning(false)
{
	title = "Engine";
	registry = std::make_unique<ECS>();
	Logger::Log("Engine created with name " + title);
	Initialize();
}

Engine::~Engine()
{
	// TODO:...
}

void Engine::Initialize()
{
	CoreInitialize();
	registry->AddSystem<RenderSystem>();
}

void Engine::CoreInitialize()
{
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(widht, height, title.c_str(), NULL, NULL);
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

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	Logger::Log("Maximum nr of vertex attributes supported: " + std::to_string(nrAttributes));

	stbi_set_flip_vertically_on_load(true);

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	isRunning = true;
}

bool Engine::IsRunning()
{
	return isRunning;
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
	registry->Update();
}

void Engine::Render()
{
	isRunning = !glfwWindowShouldClose(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	registry->GetSystem<RenderSystem>().Update();

	// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
	glfwSwapBuffers(window);
	// Check if any event like mouse/keyboard input heppen
	glfwPollEvents();
}

/// <summary>
/// Callback function to resize the actual viewport.
/// </summary>
/// <param name="window">The current GLFW window pointer.</param>
/// <param name="width">The desired width.</param>
/// <param name="height">The desired height.</param>
void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Logger::Log(std::to_string(width) + "x" + std::to_string(height));
	glViewport(0, 0, width, height);
}
