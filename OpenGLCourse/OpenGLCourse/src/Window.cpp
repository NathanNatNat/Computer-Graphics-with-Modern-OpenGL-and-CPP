// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Window.h"
#define OPENGL_VERSION_MAJOR_4 4
#define OPENGL_VERSION_MINOR_6 6

Window::Window()
{
	Width = { 800 };
	Height = { 600 };

	for (size_t i = 0; i < 1024; i++)
	{
		Keys[i] = { 0 };
	}

	XChange = { };
	YChange = { };
}

Window::Window(GLint WindowWidth, GLint WindowHeight)
{
	Width = { WindowWidth };
	Height = { WindowHeight };

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR_4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR_6);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	MainWindow = glfwCreateWindow(Width, Height, "Test Window", NULL, NULL);
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

	// Handle key and mouse input.
	CreateCallBacks();
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum Error = glewInit();
	if (Error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(Error));
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

void Window::CreateCallBacks()
{
	glfwSetKeyCallback(MainWindow, HandleKeys);
	glfwSetCursorPosCallback(MainWindow, HandleMouse);
}

GLfloat Window::GetXChange()
{
	GLfloat TheChange = XChange;
	XChange = { };
	return TheChange;
}

GLfloat Window::GetYChange()
{
	GLfloat TheChange = YChange;
	YChange = { };
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
			TheWindow->Keys[Key] = true;
			//printf("Pressed: %d\n", Key);
		}
		else if (Action == GLFW_RELEASE)
		{
			TheWindow->Keys[Key] = false;
			//printf("Released: %d\n", Key);
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, GLdouble XPos, GLdouble YPos)
{
	Window* TheWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (TheWindow->MouseFirstMoved)
	{
		TheWindow->LastX = static_cast<GLfloat>(XPos);
		TheWindow->LastY = static_cast<GLfloat>(YPos);
		TheWindow->MouseFirstMoved = false;
	}

	TheWindow->XChange = static_cast<GLfloat>(XPos) - TheWindow->LastX;
	TheWindow->YChange = TheWindow->LastY - static_cast<GLfloat>(YPos);

	TheWindow->LastX = static_cast<GLfloat>(XPos);
	TheWindow->LastY = static_cast<GLfloat>(YPos);

	//printf("x: %.6f, y:%.6f\n", TheWindow->XChange, TheWindow->YChange);
}

Window::~Window()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}
