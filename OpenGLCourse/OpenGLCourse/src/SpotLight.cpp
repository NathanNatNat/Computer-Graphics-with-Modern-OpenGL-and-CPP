// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	Direction = { glm::vec3(0.0f, -1.0f, 0.0f) };
	Edge = { };
	ProcEdge = { cosf(glm::radians(Edge)) };
	IsOn = { true };
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat near, GLfloat far, 
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, 
	GLfloat con, GLfloat lin, GLfloat exp, 
	GLfloat edg) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	Direction = { glm::normalize(glm::vec3(xDir, yDir, zDir)) };

	Edge = { edg };
	ProcEdge = { cosf(glm::radians(Edge)) };
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);

	if (IsOn)
	{
		glUniform1f(ambientIntensityLocation, AmbientIntensity);
		glUniform1f(diffuseIntensityLocation, DiffuseIntensity);
	}
	else 
	{
		glUniform1f(ambientIntensityLocation, 0.0f);
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}

	glUniform3f(positionLocation, Position.x, Position.y, Position.z);
	glUniform1f(constantLocation, Constant);
	glUniform1f(linearLocation, Linear);
	glUniform1f(exponentLocation, Exponent);

	glUniform3f(directionLocation, Direction.x, Direction.y, Direction.z);
	glUniform1f(edgeLocation, ProcEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	Position = { pos };
	Direction = { dir };
}

SpotLight::~SpotLight() { }
