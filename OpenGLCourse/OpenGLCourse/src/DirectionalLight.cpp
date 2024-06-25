#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	Direction = { glm::vec3(0.0f, -1.0f, 0.0f) };
}

DirectionalLight::DirectionalLight
(const GLuint shadowWidth, const GLuint shadowHeight, const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat aIntensity, const GLfloat dIntensity, const GLfloat xDir, const GLfloat yDir, const GLfloat zDir) 
: Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	Direction = { glm::vec3(xDir, yDir, zDir) };

	LightProj = { glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f) };
}

void DirectionalLight::UseLight(const GLint ambientIntensityLocation, const GLint ambientColourLocation, const GLint diffuseIntensityLocation, const GLint directionLocation) const
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);

	glUniform3f(directionLocation, Direction.x, Direction.y, Direction.z);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform() const
{
	return LightProj * glm::lookAt(-Direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() = default;
