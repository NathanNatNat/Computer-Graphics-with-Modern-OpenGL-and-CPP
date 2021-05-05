// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 Direction{ };
};
