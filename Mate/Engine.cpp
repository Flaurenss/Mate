#include "Engine.h"
#include <iostream>
#include "Logger.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "stb_image.h"
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "Model.h"
#include "CameraComponent.h"
#include "Input.h"
#include "PhysicsSystem.h"
#include "DebugDraw.h"
#include "AnimationSystem.h"
#include "AudioSystem.h"
#include "AssetManager.h"
#include "Skybox.h"

float Engine::DeltaTime = 0;

Engine::Engine(int w, int h, std::string title) :
	isRunning(false),
	lastFrame(0),
	title(title)
{
	renderContext = RenderContext
	{
		Matrix4(),
		Matrix4(),
		w,
		h
	};

	registry = std::make_unique<ECS>();
	Logger::Log("Engine created with name " + title);

	Initialize();
}

Engine::~Engine()
{
	delete window;
	window = nullptr;
}

void Engine::Initialize()
{
	CoreInitialize();
	DebugDraw::Init();
	Shader shader("./Assets/vertexShader.shader", "./Assets/fragmentShader.shader");
	Shader skyboxShader("./Assets/vsSkybox.shader", "./Assets/fsSkybox.shader");
	registry->AddSystem<PhysicsSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<RenderSystem>(shader, skyboxShader);
	registry->AddSystem<AudioSystem>();
	registry->AddSystem<CameraSystem>();
}

void Engine::CoreInitialize()
{
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(renderContext.Width, renderContext.Height, title.c_str(), NULL, NULL);

	Input::Init(window);

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

	stbi_set_flip_vertically_on_load(false);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set engine isntance to window
	glfwSetWindowUserPointer(window, this);
	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	//glfwSetCursorPosCallback(window, mouse_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	isRunning = true;
}

void Engine::Framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	Logger::Log(std::to_string(w) + "x" + std::to_string(h));
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->renderContext.Width = w;
	engine->renderContext.Height = h;
	glViewport(0, 0, w, h);
}

bool Engine::IsRunning()
{
	return isRunning;
}

void Engine::Update()
{
	ComputeDelta();
	DebugFps(DeltaTime);
	
	Input::Update();
	testProcessInput(window);

	registry->Update();
	RenderUpdate();
}

void Engine::SetSimulationTo(bool status)
{
	runSimulation = status;
}

Entity Engine::CreateEntity()
{
	return registry->CreateEntity();
}

void Engine::SetSkybox(const std::string& id, std::array<std::string, 6> faces)
{
	auto texture = AssetManager::GetInstance().LoadCubemap(id, faces);
	auto skybox = std::make_unique<Skybox>(texture);
	if (registry->HasSystem<RenderSystem>())
	{
		registry->GetSystem<RenderSystem>().SetSkybox(std::move(skybox));
	}
}

void Engine::SetRenderDebugMode(bool mode)
{
	renderContext.DebugMode = mode;
}

void Engine::ComputeDelta()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Engine::PhysicsUpdate(float fixedDeltaTime)
{
	registry->GetSystem<PhysicsSystem>().Update(fixedDeltaTime);
}

void Engine::RenderUpdate()
{
	isRunning = !glfwWindowShouldClose(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	registry->GetSystem<CameraSystem>().Update(renderContext);
	registry->GetSystem<AnimationSystem>().Update(DeltaTime);
	registry->GetSystem<RenderSystem>().Update(renderContext);
	registry->GetSystem<AudioSystem>().Update();

	// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
	glfwSwapBuffers(window);
	// Check if any event like mouse/keyboard input heppen
	glfwPollEvents();
}

void Engine::DebugFps(float deltaTime)
{
	float smoothedFPS = 0.0f;

	frameTimeAccumulator += DeltaTime;
	frameCount++;
	if (frameTimeAccumulator >= 1.0f) {
		smoothedFPS = frameCount / frameTimeAccumulator;
		frameTimeAccumulator = 0.0f;
		frameCount = 0;
		Logger::War("FPS: " + std::to_string(smoothedFPS));
	}
}

void Engine::testProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
