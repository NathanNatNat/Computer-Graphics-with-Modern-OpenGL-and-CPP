// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SpotLight.h"

SpotLight::SpotLight()
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

void SpotLight::UseLight(const GLuint ambientIntensityLocation, const GLuint ambientColourLocation,
                         const GLuint diffuseIntensityLocation, const GLuint positionLocation, const GLuint directionLocation,
                         const GLuint constantLocation, const GLuint linearLocation, const GLuint exponentLocation,
                         const GLuint edgeLocation) const
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

SpotLight::~SpotLight() = default;
