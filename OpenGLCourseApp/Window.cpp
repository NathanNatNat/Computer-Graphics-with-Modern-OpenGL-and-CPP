// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = { 0 };
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Windows Properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Handle key and mouse input.
	CreateCallBacks();
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::CreateCallBacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

GLdouble Window::GetXChange()
{
	GLdouble TheChange = XChange;
	XChange = 0.0f;
	return TheChange;
}

GLdouble Window::GetYChange()
{
	GLdouble TheChange = YChange;
	YChange = 0.0f;
	return TheChange;
}

void Window::HandleKeys(GLFWwindow* window, int Key, int Code, int Action, int Mode)
{
	Window* TheWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (Key >= 0 && Key < 1024)
	{
		if (Action == GLFW_PRESS)
		{
			TheWindow->keys[Key] = true;
			//printf("Pressed: %d\n", Key);
		}
		else if (Action == GLFW_RELEASE)
		{
			TheWindow->keys[Key] = false;
			//printf("Released: %d\n", Key);
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, double XPos, double YPos)
{
	Window* TheWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (TheWindow->MouseFirstMoved)
	{
		TheWindow->LastX = XPos;
		TheWindow->LastY = YPos;
		TheWindow->MouseFirstMoved = false;
	}

	TheWindow->XChange = XPos - TheWindow->LastX;
	TheWindow->YChange = TheWindow->LastY - YPos;

	TheWindow->LastX = XPos;
	TheWindow->LastY = YPos;

	//printf("x: %.6f, y:%.6f\n", TheWindow->XChange, TheWindow->YChange);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
