// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Window.h"
#include <cstdio>

Window::Window()
{
	Width = { 800 };
	Height = { 600 };

	for (bool& Key : Keys)
	{
		Key = { };
	}
	
	XChange = { };
	YChange = { };
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	Width = { windowWidth };
	Height = { windowHeight };

	for (bool& Key : Keys)
	{
		Key = { };
	}
	
	XChange = { };
	YChange = { };
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	MainWindow = { glfwCreateWindow(Width, Height, "OpenGL Course", nullptr, nullptr) };
	if (!MainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(MainWindow, &BufferWidth, &BufferHeight);

	// Set the current context
	glfwMakeContextCurrent(MainWindow);

	// Handle Key + Mouse Input
	CreateCallbacks();
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = { GL_TRUE };

	if (const GLenum Error = { glewInit() }; Error != GLEW_OK)
	{
		printf("Error: %p", glewGetErrorString(Error));
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	glfwSetWindowUserPointer(MainWindow, this);

	return 0;
}

void Window::CreateCallbacks() const
{
	glfwSetKeyCallback(MainWindow, HandleKeys);
	glfwSetCursorPosCallback(MainWindow, HandleMouse);
}

GLdouble Window::GetXChange()
{
	const GLdouble TheChange = { XChange };
	XChange = { };
	return TheChange;
}

GLdouble Window::GetYChange()
{
	const GLdouble TheChange = { YChange };
	YChange = { };
	return TheChange;
}

void Window::HandleKeys(GLFWwindow* window, const int key, int code, const int action, int mode)
{
	Window* TheWindow = { static_cast<Window*>(glfwGetWindowUserPointer(window)) };

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			TheWindow->Keys[key] = { true };
		}
		else if (action == GLFW_RELEASE)
		{
			TheWindow->Keys[key] = { false };
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, GLdouble xPos, GLdouble yPos)
{
	Window* TheWindow = { static_cast<Window*>(glfwGetWindowUserPointer(window)) };

	if (TheWindow->MouseFirstMoved)
	{
		TheWindow->LastX = { xPos };
		TheWindow->LastY = { yPos };
		TheWindow->MouseFirstMoved = { false };
	}

	TheWindow->XChange = { xPos - TheWindow->LastX };
	TheWindow->YChange = { TheWindow->LastY - yPos };

	TheWindow->LastX = { xPos };
	TheWindow->LastY = { yPos };
}

Window::~Window()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}
