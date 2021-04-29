// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Light.h"

Light::Light()
{
	Colour = { glm::vec3(1.0f, 1.0f, 1.0f) };
	AmbientIntensity = { 1.0f };
	DiffuseIntensity = { 0.0f };
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	Colour = { glm::vec3(red, green, blue) };
	AmbientIntensity = { aIntensity };
	DiffuseIntensity = { dIntensity };

	shadowMap = { new ShadowMap() };
	shadowMap->Init(shadowWidth, shadowHeight);
}

Light::~Light() { }
