// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Light.h"

Light::Light()
{
	Colour = { glm::vec3(1.0f, 1.0f, 1.0f) };
	AmbientIntensity = { 1.0f };
	DiffuseIntensity = { };
}

Light::Light(const GLuint shadowWidth, const GLuint shadowHeight, const GLfloat red, const GLfloat green, const GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	Colour = { glm::vec3(red, green, blue) };
	AmbientIntensity = { aIntensity };
	DiffuseIntensity = { dIntensity };

	shadowMap = { new ShadowMap() };
	shadowMap->Init(shadowWidth, shadowHeight);
}

Light::~Light() = default;
