// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Window dimensions
const int WIDTH{ 800 }, HEIGHT{ 600 };

uint32_t VBO{ }, VAO{ }, IBO{ }, shader{ }, uniformModel{ }, uniformProjection{ };

bool direction{ true };
float triOffset{ 0.0f };
float triMaxOffset{ 0.7f };
float triIncrement{ 0.0005f };
float CurrentAngle{ 0.0f };

bool SizeDirection{ true };
float CurrentSize{ 0.4f };
float MaxSize{ 0.8f };
float MinSize{ 0.1f };

// Vertex Shader code
static const char* vShader{ "							\n\
#version 330											\n\
														\n\
layout (location = 0) in vec3 pos;						\n\
														\n\
out vec4 VColour;										\n\
														\n\
uniform mat4 model;										\n\
uniform mat4 projection;								\n\
														\n\
void main()												\n\
{														\n\
	gl_Position = projection * model * vec4(pos, 1.0);	\n\
	VColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);		\n\
}" };

// Fragment Shader
static const char* fShader{ "						\n\
#version 330										\n\
													\n\
in vec4 VColour;									\n\
													\n\
out vec4 colour;									\n\
													\n\
void main()											\n\
{													\n\
	colour = VColour;								\n\
}" };

void CreateTriangle()
{
	unsigned int Indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	float vertices[] = 
	{
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void AddShader(uint32_t theProgram, const char* shaderCode, int shaderType)
{
	uint32_t theShader{ glCreateShader(shaderType) };

	const char* theCode[1]{ };
	theCode[0] = { shaderCode };

	glShaderSource(theShader, 1, theCode, NULL);
	glCompileShader(theShader);

	int result{ 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		char eLog[1024]{ 0 };
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = { glCreateProgram() };

	if (!shader)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	int result{ };
	char eLog[1024]{ 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

	// Create the window
	GLFWwindow* mainWindow{ glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL) };
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth{ }, bufferHeight{ };
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = { true };

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport.
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (float)bufferWidth / (float)bufferHeight, 0.1f, 100.0f);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		CurrentAngle += 1.0f; // ???? The course has this set to 0.001f, which causes it to rotate a lot slower for me.
		if (CurrentAngle >= 360)
		{
			CurrentAngle -= 360;
		}

		if (SizeDirection)
		{
			CurrentSize += 0.0001f;
		}
		else
		{
			CurrentSize -= 0.0001f;
		}

		if ((CurrentSize >= MaxSize) || (CurrentSize <= MinSize))
		{
			SizeDirection = { !SizeDirection };
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, glm::radians(CurrentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, false, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
