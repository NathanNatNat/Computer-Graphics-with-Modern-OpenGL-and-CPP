// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }

	GLdouble GetXChange();
	GLdouble GetYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow{ };

	bool
		keys[1024]{ },
		MouseFirstMoved{ };

	GLint 
		width{ }, 
		height{ },
		bufferWidth{ }, 
		bufferHeight{ };

	GLdouble
		LastX{ }, 
		LastY{ }, 
		XChange{ }, 
		YChange{ };
	
	void CreateCallBacks();
	static void HandleKeys(GLFWwindow* Window, int Key, int Code, int Action, int Mode);
	static void HandleMouse(GLFWwindow* window, double XPos, double YPos);
};
