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

	Window(GLint WindowWidth, GLint WindowHeight);

	int Initialise();

	GLint GetBufferWidth() { return BufferWidth; }
	GLint GetBufferHeight() { return BufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(MainWindow); }

	bool* GetKeys() { return Keys; }

	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() { glfwSwapBuffers(MainWindow); }

	~Window();

private:
	GLFWwindow* MainWindow{ };

	bool
		Keys[1024]{ },
		MouseFirstMoved{ };

	GLint 
		Width{ }, 
		Height{ },
		BufferWidth{ }, 
		BufferHeight{ };

	GLfloat
		LastX{ }, 
		LastY{ }, 
		XChange{ }, 
		YChange{ };
	
	void CreateCallBacks();
	static void HandleKeys(GLFWwindow* Window, int Key, int Code, int Action, int Mode);
	static void HandleMouse(GLFWwindow* window, GLdouble XPos, GLdouble YPos);
};
