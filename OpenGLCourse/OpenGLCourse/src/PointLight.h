// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
		GLint diffuseIntensityLocation, GLint positionLocation,
		GLint constantLocation, GLint linearLocation, GLint exponentLocation);

	~PointLight();

private:
	glm::vec3 Position;

	GLfloat Constant, Linear, Exponent;
};
