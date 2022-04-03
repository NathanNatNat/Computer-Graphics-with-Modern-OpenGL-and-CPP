// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	Direction = { glm::vec3(0.0f, -1.0f, 0.0f) };
}

DirectionalLight::DirectionalLight
(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir) 
: Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	Direction = { glm::vec3(xDir, yDir, zDir) };

	LightProj = { glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f) };
}

void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);

	glUniform3f(directionLocation, Direction.x, Direction.y, Direction.z);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return LightProj * glm::lookAt(-Direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() = default;
