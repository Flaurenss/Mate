#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int vertexShader();
unsigned int fragmentShader();
unsigned int createShaderProgram();
void checkErrors(unsigned int vertexShader);
void checkShaderProgramErrors(unsigned int shaderProgram);
void checkShaderProgramErrors(unsigned int shaderProgram);

const char* TITLE = "Dobro";
const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
	glfwInit();

	// OpenGl version to use, if user don't have it set it, will fail.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	// build and compile our shader program
	// ------------------------------------
	unsigned int shaderProgram = createShaderProgram();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom left  
		 0.5f, -0.5f, 0.0f, // bottom right 
		-0.5f,  0.5f, 0.0f, // top left
		 0.5f, 0.5f, 0.0f  // top right
	};
	unsigned int indices[] =
	{
		0, 1, 2,
		1, 2, 3,
	};

	// Declare:
	// Vertex Array Object
	// Vertex Buffer Object
	// Element Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Binds the current VBO to the actual VAO and tells how to be interpret it. 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO, VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// EBO must be unbind after VAO, otherwise VAO will be updated.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set rendering viewport callback
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
	
	// Draw primitives configuration
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Frame loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render commands
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		// Swap buffers, front for already rendered colors an the back one in order to avoid artifacts.
		glfwSwapBuffers(window);
		// Check if any event like mouse/keyboard input heppen
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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
}

/// <summary>
/// Creates Vertex Shader Object
/// </summary>
unsigned int vertexShader()
{
	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderId);

	checkErrors(vertexShaderId);

	return vertexShaderId;
}

unsigned int fragmentShader()
{
	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderId);

	checkErrors(fragmentShaderId);

	return fragmentShaderId;
}

unsigned int createShaderProgram()
{
	auto vertexShaderId = vertexShader();
	auto fragmentShaderId = fragmentShader();

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShaderId);
	glAttachShader(shaderProgram, fragmentShaderId);

	glLinkProgram(shaderProgram);

	checkShaderProgramErrors(shaderProgram);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return shaderProgram;
}

void checkErrors(unsigned int shaderId) {
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
	if (!success)
	{
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "GG::SHADER::VERTEX::COMPILATION_SUCCESSFULL\n" << infoLog << std::endl;
	}
}

void checkShaderProgramErrors(unsigned int shaderProgram) {
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	if (!success)
	{
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "GG::SHADER::PROGRAM::COMPILATION_SUCCESSFULL" << infoLog << std::endl;
	}
}