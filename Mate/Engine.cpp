#include "Engine.h"
#include <iostream>
#include "Logger.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "stb_image.h"
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "Camera.h"
#include "Model.h"

int Engine::width = 1920;
int Engine::height = 1080;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Engine::Engine(int w, int h) :
	isRunning(false)
{
	width = w;
	height = h;

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
	Shader shader("./vertexShader.shader", "./fragmentShader.shader");
	registry->AddSystem<RenderSystem>(shader);
	registry->AddSystem<CameraSystem>(shader);
}

void Engine::CoreInitialize()
{
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	isRunning = true;
}

void Engine::Framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	Logger::Log(std::to_string(w) + "x" + std::to_string(h));
	glViewport(0, 0, width, height);
}

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Vector3 cameraPos = Vector3(0.0f, 0.0f, 3.0f);
Vector3 cameraFront = Vector3(0.0f, 0.0f, -1.0f);
Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos, cameraUp);

void Engine::Test()
{
	auto model = new Model("E:/TFG/Models/Kenney/Models/GLB format/character-female-d.glb");
	//auto model = new Model("E:/TFG/Models/glTF-Sample-Assets-main/Models/Duck/glTF/Duck.gltf");


	// Hide mouse - focus mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	// Set mouse movement callback
	//glfwSetCursorPosCallback(window, mouse_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Custom Shader class:
	Shader myShader("./vertexShader.shader", "./fragmentShader.shader");

	// Frame loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.Use();

		Matrix4 view = camera.GetViewMatrix();

		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.get());

		Matrix4 projection = Matrix4();
		projection.perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.get());

		Matrix4 modelMatrix = Matrix4();
		//modelMatrix.rotate(-180, Vector3(0, 1.0f, 0));
		//modelC.scale(0.15f);
		/*modelC.translate();*/
		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix.get());

		model->Draw(myShader);

		// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
		glfwSwapBuffers(window);
		// Check if any event like mouse/keyboard input heppen
		glfwPollEvents();
	}

	glfwTerminate();
}

bool Engine::IsRunning()
{
	return isRunning;
}

void Engine::ProcessInput()
{
}

void Engine::Run()
{
	while (isRunning)
	{
		//ProcessInput();
		Update();
		Render();
	}
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

	registry->GetSystem<CameraSystem>().Update();
	registry->GetSystem<RenderSystem>().Update();

	// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
	glfwSwapBuffers(window);
	// Check if any event like mouse/keyboard input heppen
	glfwPollEvents();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
/// <summary>
/// Process user input given a GLFW window.
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(RIGHT, deltaTime);
}
