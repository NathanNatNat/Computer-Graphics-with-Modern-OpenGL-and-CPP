// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 StartPosition, glm::vec3 StartUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMovementSpeed, GLfloat StartTurnSpeed)
{
	Position = StartPosition;
	WorldUp = StartUp;
	Yaw = StartYaw;
	Pitch = StartPitch;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = StartMovementSpeed;
	TurnSpeed = StartTurnSpeed;

	Update();
}

void Camera::KeyControl(const bool* Keys, GLfloat DeltaTime)
{
	GLfloat Velocity = MovementSpeed * DeltaTime;

	if (Keys[GLFW_KEY_W])
	{
		Position += Front * Velocity;
	}

	if (Keys[GLFW_KEY_S])
	{
		Position -= Front * Velocity;
	}

	if (Keys[GLFW_KEY_A])
	{
		Position -= Right * Velocity;
	}

	if (Keys[GLFW_KEY_D])
	{
		Position += Right * Velocity;
	}
}

void Camera::MouseControl(GLfloat XChange, GLfloat YChange)
{
	XChange *= TurnSpeed;
	YChange *= TurnSpeed;

	Yaw += XChange;
	Pitch += YChange;

	if (Pitch > 89.0f)
	{
		Pitch = 89.0f;
	}

	if (Pitch < -89.0f)
	{
		Pitch = -89.0f;
	}

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return Position;
}

void Camera::Update()
{
	Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(Front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

Camera::~Camera()
{

}
