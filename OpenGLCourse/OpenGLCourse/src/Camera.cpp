// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Camera.h"

Camera::Camera() { }

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	Position = { startPosition };
	WorldUp = { startUp };
	Yaw = { startYaw };
	Pitch = { startPitch };
	Front = { glm::vec3(0.0f, 0.0f, -1.0f) };

	MoveSpeed = { startMoveSpeed };
	TurnSpeed = { startTurnSpeed };

	Update();
}

void Camera::KeyControl(bool* keys, GLdouble deltaTime)
{
	GLdouble Velocity = MoveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		Position += Front * Velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		Position -= Front * Velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		Position -= Right * Velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		Position += Right * Velocity;
	}
}

void Camera::MouseControl(GLdouble xChange, GLdouble yChange)
{
	xChange *= TurnSpeed;
	yChange *= TurnSpeed;

	Yaw += xChange;
	Pitch += yChange;

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

glm::vec3 Camera::GetCameraDirection()
{
	return glm::normalize(Front);
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

Camera::~Camera() { }
