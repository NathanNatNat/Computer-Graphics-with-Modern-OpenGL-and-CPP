// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* VertexCode, const char* FragmentCode);
	void CreateFromFiles(const char* VertexLocation, const char* FragmentLocation);

	std::string ReadFile(const char* FileLocation);

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

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int PointLightCount{ };

	GLuint
		ShaderID{ },
		UniformProjection{ },
		UniformModel{ },
		UniformView{ },
		UniformEyePosition{ },
		UniformSpecularIntensity{ },
		UniformShininess{ };

	struct 
	{
		GLuint 
			UniformColour{ },
			UniformAmbientIntensity{ },
			UniformDiffuseIntensity{ },
			UniformDirection{ };

	} UniformDirectionalLight{ };

	GLuint UniformPointLightCount{ };

	struct 
	{
		GLuint
			UniformColour,
			UniformAmbientIntensity,
			UniformDiffuseIntensity,
			UniformPosition,
			UniformConstant,
			UniformLinear,
			UniformExponent;

	} UniformPointLight[MAX_POINT_LIGHTS]{ };

	void CompileShader(const char* VertexCode, const char* FragmentCode);
	void AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType);
};
