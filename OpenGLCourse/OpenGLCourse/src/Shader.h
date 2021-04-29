// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight * pLight, int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight * sLight, GLint lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount{ };
	int spotLightCount{ };

	GLuint shaderID{ }, UniformProjection{ }, UniformModel{ }, UniformView{ }, UniformEyePosition{ },
		UniformSpecularIntensity{ }, UniformShininess{ },
		UniformTexture{ }, UniformDirectionalShadowMap{ },
		UniformDirectionalLightTransform{ },
		UniformOmniLightPos{ }, UniformFarPlane{ };
	
	GLuint UniformLightMatrices[6]{ };

	struct {
		GLuint UniformColour{ };
		GLuint UniformAmbientIntensity{ };
		GLuint UniformDiffuseIntensity{ };

		GLuint UniformDirection{ };
	} UniformDirectionalLight{ };

	GLuint UniformPointLightCount{ };

	struct {
		GLuint UniformColour{ };
		GLuint UniformAmbientIntensity{ };
		GLuint UniformDiffuseIntensity{ };

		GLuint UniformPosition{ };
		GLuint UniformConstant{ };
		GLuint UniformLinear{ };
		GLuint UniformExponent{ };
	} UniformPointLight[MAX_POINT_LIGHTS]{ };

	GLuint UniformSpotLightCount{ };

	struct {
		GLuint UniformColour{ };
		GLuint UniformAmbientIntensity{ };
		GLuint UniformDiffuseIntensity{ };

		GLuint UniformPosition{ };
		GLuint UniformConstant{ };
		GLuint UniformLinear{ };
		GLuint UniformExponent{ };

		GLuint UniformDirection{ };
		GLuint UniformEdge{ };
	} UniformSpotLight[MAX_SPOT_LIGHTS]{ };

	struct {
		GLuint shadowMap{ };
		GLuint farPlane{ };
	} UniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS]{ };

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};
