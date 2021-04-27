// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Light.h"

Light::Light()
{
	Colour = { glm::vec3(1.0f, 1.0f, 1.0f) };
	AmbientIntensity = { 1.0f };
	Direction = { glm::vec3(0.0f, -1.0f, 0.0f) };
	DiffuseIntensity = { 0.0f };
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	Colour = { glm::vec3(red, green, blue) };
	AmbientIntensity = { aIntensity };
	Direction = { glm::vec3(xDir, yDir, zDir) };
	DiffuseIntensity = { dIntensity };
}

void Light::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);

	glUniform3f(directionLocation, Direction.x, Direction.y, Direction.z);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);
}

Light::~Light()
{

}
