// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();

	Camera(glm::vec3 StartPosition, glm::vec3 StartUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMoveSpeed, GLfloat StartTurnSpeed);

	void KeyControl(const bool* Keys, GLfloat DeltaTime);
	void MouseControl(GLfloat XChange, GLfloat YChange);

	glm::vec3 GetCameraPosition();
	glm::mat4 CalculateViewMatrix();

	~Camera();

private:
	glm::vec3
		Position{ },
		Front{ },
		Up{ },
		Right{ },
		WorldUp{ };

	GLfloat
		Yaw{ },
		Pitch{ },
		MovementSpeed{ },
		TurnSpeed{ };

	void Update();
};
