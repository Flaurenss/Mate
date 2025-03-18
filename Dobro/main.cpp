#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include "Shader.h"
#include <Vector.h>
#include <Matrix.h>
#include <MathUtils.h>
#include "Camera.h"

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const char* TITLE = "Dobro";
const float WIDTH = 800;
const float HEIGHT = 600;
const float DEG2RAD = 3.141593f / 180.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Mouse params in the middle of the screen
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

Vector3 cameraPos = Vector3(0.0f, 0.0f, 3.0f);
Vector3 cameraFront = Vector3(0.0f, 0.0f, -1.0f);
Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

int main() {
	glfwInit();
	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Btw Intermediate/Core profile, we set the last one:
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW widnow" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// build and compile our shader program
	// ------------------------------------
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	//unsigned int shaderProgram = createShaderProgram();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Declare:
	// Vertex Array Object
	// Vertex Buffer Object
	// Element Buffer Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Binds the current VBO to the actual VAO and tells how to be interpret it. 
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	//Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Vertex color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	 
	// Vertex texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind VBO, VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// EBO must be unbind after VAO, otherwise VAO will be updated.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//===================== TEXTURES =====================//
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set wrapping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set filtering method:
	// When scaling up we want interpolation values (in order to avoid pixelated effect).
	// When scaling down we want precise values in the order of MinMaps.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("./cat.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

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
	Shader myShader("./vertexShader.vs", "./fragmentShader.fs");
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);

	Vector3 cubePositions[] = {
		Vector3(0.0f,  0.0f,  0.0f),
		Vector3(2.0f,  5.0f, -15.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -12.3f),
		Vector3(2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3(1.3f, -2.0f, -2.5f),
		Vector3(1.5f,  2.0f, -2.5f),
		Vector3(1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)
	};
	Camera* camera = new Camera(cameraPos, cameraUp);
	// Frame loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << deltaTime << std::endl;
		// input
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render commands
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue)/2) + 0.5f;
		
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.use();
		glBindVertexArray(VAO);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 1; i++)
		{
			float angle = 20.0f * i;
			auto normalizedC = Vector3(0.0f, 0.0f, 1.0f).normalize();

			Matrix4 modelC = Matrix4().identity();
			modelC.rotate(angle, normalizedC);
			modelC.translate(cubePositions[i]);
			
			/*Vector3 cubePos = cameraPos + Vector3(0, -4.0f, cameraFront * 1.5f);
			modelC.translate(cubePos);*/

			unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelC.get());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		Matrix4 view = camera->GetViewMatrix();

		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.get());
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		auto projection = Matrix4();
		projection.perspective(45.0f, WIDTH/HEIGHT, 0.1f, 100.0f);

		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.get());
		
		// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
		glfwSwapBuffers(window);
		// Check if any event like mouse/keyboard input heppen
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

/// <summary>
/// Callback function to resize the actual viewport.
/// </summary>
/// <param name="window">The current GLFW window pointer.</param>
/// <param name="width">The desired width.</param>
/// <param name="height">The desired height.</param>
void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << width << "x" << height << std::endl;
	glViewport(0, 0, width, height);
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
	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= cameraSpeed * (Vector3::cross(cameraFront, cameraUp)).normalize();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += cameraSpeed * (Vector3::cross(cameraFront, cameraUp)).normalize();
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
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

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 80.0f)
		pitch = 80.0f;
	if (pitch < -80.0f)
		pitch = -80.0f;

	Vector3 direction = Vector3();
	direction.x = cos(MathUtils::radians(yaw)) * cos(MathUtils::radians(pitch));
	direction.y = sin(MathUtils::radians(pitch));
	direction.z = sin(MathUtils::radians(yaw)) * cos(MathUtils::radians(pitch));
	cameraFront = direction.normalize();
	//cameraFront = glm::vec3(direction.x, direction.y, direction.z);
}
