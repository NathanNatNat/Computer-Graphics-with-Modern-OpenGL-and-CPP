// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 Colour{ };

	GLfloat 
		AmbientIntensity{ },
		DiffuseIntensity{ };

	glm::mat4 LightProj{ };

	ShadowMap* shadowMap{ };
};
