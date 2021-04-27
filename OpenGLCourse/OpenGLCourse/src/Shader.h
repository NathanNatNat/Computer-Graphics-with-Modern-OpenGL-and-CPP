// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>

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

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint
		ShaderID{ },
		UniformProjection{ },
		UniformModel{ },
		UniformView{ },
		UniformAmbientIntensity{ },
		UniformAmbientColour{ },
		UniformDiffuseIntensity{ },
		UniformDirection{ };

	void CompileShader(const char* VertexCode, const char* FragmentCode);
	void AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType);
};
