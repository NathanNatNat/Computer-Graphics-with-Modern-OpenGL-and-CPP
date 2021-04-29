// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PointLight.h"

PointLight::PointLight() : Light()
{
	Position = { glm::vec3(0.0f, 0.0f, 0.0f) };
	Constant = { 1.0f };
	Linear = { };
	Exponent = { };
}

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
						GLfloat near, GLfloat far, 
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	Position = { glm::vec3(xPos, yPos, zPos) };
	Constant = { con };
	Linear = { lin };
	Exponent = { exp };

	FarPlane = { far };

	float Aspect = { static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight) };
	LightProj = { glm::perspective(glm::radians(90.0f), Aspect, near, far) };

	shadowMap = { new OmniShadowMap() };
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColourLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(ambientIntensityLocation, AmbientIntensity);
	glUniform1f(diffuseIntensityLocation, DiffuseIntensity);

	glUniform3f(positionLocation, Position.x, Position.y, Position.z);
	glUniform1f(constantLocation, Constant);
	glUniform1f(linearLocation, Linear);
	glUniform1f(exponentLocation, Exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
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

GLfloat PointLight::GetFarPlane()
{
	return FarPlane;
}

glm::vec3 PointLight::GetPosition()
{
	return Position;
}

PointLight::~PointLight() { }
