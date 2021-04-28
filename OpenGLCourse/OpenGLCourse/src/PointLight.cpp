// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PointLight.h"

PointLight::PointLight() : Light()
{
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Constant = 1.0f;
	Linear = 0.0f;
	Exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	Position = glm::vec3(xPos, yPos, zPos);
	Constant = con;
	Linear = lin;
	Exponent = exp;
}

void PointLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
	GLint diffuseIntensityLocation, GLint positionLocation,
	GLint constantLocation, GLint linearLocation, GLint exponentLocation)
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);

	glUniform3f(positionLocation, Position.x, Position.y, Position.z);
	glUniform1f(constantLocation, Constant);
	glUniform1f(linearLocation, Linear);
	glUniform1f(exponentLocation, Exponent);
}

PointLight::~PointLight()
{
}
