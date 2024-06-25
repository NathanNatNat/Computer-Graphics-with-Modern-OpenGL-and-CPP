// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PointLight.h"

PointLight::PointLight()
{
	Position = { glm::vec3(0.0f, 0.0f, 0.0f) };
	Constant = { 1.0f };
	Linear = { };
	Exponent = { };
}

PointLight::PointLight(const GLuint shadowWidth, const GLuint shadowHeight,
                       const GLfloat near, GLfloat far,
                       const GLfloat red, const GLfloat green, const GLfloat blue,
                       const GLfloat aIntensity, const GLfloat dIntensity,
                       const GLfloat xPos, const GLfloat yPos, const GLfloat zPos, 
                       GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	Position = { glm::vec3(xPos, yPos, zPos) };
	Constant = { con };
	Linear = { lin };
	Exponent = { exp };

	FarPlane = { far };

	const float Aspect = { static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight) };
	LightProj = { glm::perspective(glm::radians(90.0f), Aspect, near, far) };

	shadowMap = { new OmniShadowMap() };
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(const GLuint ambientIntensityLocation, const GLuint ambientColourLocation,
                          const GLuint diffuseIntensityLocation, const GLuint positionLocation,
                          const GLuint constantLocation, const GLuint linearLocation, const GLuint exponentLocation) const
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);

	glUniform3f(positionLocation, Position.x, Position.y, Position.z);
	glUniform1f(constantLocation, Constant);
	glUniform1f(linearLocation, Linear);
	glUniform1f(exponentLocation, Exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform() const
{
	std::vector<glm::mat4> LightMatrices{ };
	LightMatrices.reserve(6);
	//+x, -x
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//+z, -z
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	LightMatrices.push_back(LightProj * glm::lookAt(Position, Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return LightMatrices;
}

GLfloat PointLight::GetFarPlane() const
{
	return FarPlane;
}

glm::vec3 PointLight::GetPosition() const
{
	return Position;
}

PointLight::~PointLight() = default;
