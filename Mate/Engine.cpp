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
#include "CameraComponent.h"

Engine* Engine::instance = nullptr;
int Engine::width = 1920;
int Engine::height = 1080;
float Engine::DeltaTime = 0;
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

Engine::Engine(int w, int h) :
	isRunning(false),
	lastFrame(0)
{
	width = w;
	height = h;

	title = "Engine";
	registry = std::make_unique<ECS>();
	Logger::Log("Engine created with name " + title);

	Vector3 cameraPos = Vector3(0.0f, 1.0f, 3.0f);
	//Vector3 cameraFront = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

	testCamera = new Camera(cameraPos, cameraUp);

	Initialize();
}

Engine::~Engine()
{
	// TODO:...
}

void Engine::Initialize()
{
	instance = this;
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	isRunning = true;

	auto cameraTest = registry->CreateEntity();
	cameraTest.AddComponent<TransformComponent>();
	cameraTest.AddComponent<CameraComponent>(testCamera);
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

float testDeltaTime = 0.0f;	// Time between current frame and last frame
float testLastFrame = 0.0f; // Time of last frame

Vector3 cameraPos = Vector3(0.0f, 0.0f, 3.0f);
Vector3 cameraFront = Vector3(0.0f, 0.0f, -1.0f);
Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos, cameraUp);

void Engine::Test()
{
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "TEST", NULL, NULL);
	if (window == NULL)
	{
		Logger::Err("Failed to create GLFW widnow");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::Err("Failed to initialize GLAD");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// build and compile our shader program
	// ------------------------------------
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	Logger::Log("Maximum nr of vertex attributes supported: " + std::to_string(nrAttributes));
	stbi_set_flip_vertically_on_load(false);

	auto model = new Model("C:/Users/loren/Downloads/glTF-Sample-Models-main/glTF-Sample-Models-main/2.0/Avocado/glTF/Avocado.gltf");
	//auto model = new Model("E:/TFG/Models/Kenney/Models/GLB format/character-female-d.glb");
	//auto model = new Model("E:/TFG/Models/glTF-Sample-Assets-main/Models/Duck/glTF/Duck.gltf");

	// Hide mouse - focus mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	// Set mouse movement callback
	glfwSetCursorPosCallback(window, mouse_callback);

	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Custom Shader class:
	Shader myShader("./vertexShader.shader", "./fragmentShader.shader");

	// Frame loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		testDeltaTime = currentFrame - testLastFrame;
		testLastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLenum err;
		/*while ((err = glGetError()) == GL_NO_ERROR)
		{
		}*/
		err = glGetError();
		if (err != GL_NO_ERROR)
		{
			Logger::Err("OpenGL error: " + std::to_string(err));
		}

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
		modelMatrix.translate(Vector3(2, 0, 0));
		modelMatrix.rotate(testDeltaTime * 20.0f, Vector3(0, 1.0f, 0));
		//modelC.scale(0.15f);
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
	float currentFrame = static_cast<float>(glfwGetTime());
	DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	testProcessInput(window);

	registry->Update();
}

void Engine::Render()
{
	isRunning = !glfwWindowShouldClose(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	registry->GetSystem<CameraSystem>().Update();
	registry->GetSystem<RenderSystem>().Update();

	// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
	glfwSwapBuffers(window);
	// Check if any event like mouse/keyboard input heppen
	glfwPollEvents();
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

	if (instance) {
		instance->testMouse_callback(window, xpos, ypos);
	}
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
	{
		camera.ProcessKeyboardMovement(FORWARD, testDeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboardMovement(BACKWARD, testDeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboardMovement(LEFT, testDeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboardMovement(RIGHT, testDeltaTime);
	}
}

void Engine::testProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		testCamera->ProcessKeyboardMovement(FORWARD, DeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		testCamera->ProcessKeyboardMovement(BACKWARD, DeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		testCamera->ProcessKeyboardMovement(LEFT, DeltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		testCamera->ProcessKeyboardMovement(RIGHT, DeltaTime);
	}
}

void Engine::testMouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (testFirstMouse)
	{
		testLastX = xpos;
		testLastY = ypos;
		testFirstMouse = false;
	}

	float xoffset = xpos - testLastX;
	float yoffset = testLastY - ypos; // reversed since y-coordinates range from bottom to top
	testLastX = xpos;
	testLastY = ypos;

	testCamera->ProcessMouseMovement(xoffset, yoffset);
}
