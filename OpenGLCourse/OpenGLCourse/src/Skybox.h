// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox();

	Skybox(const std::vector<std::string>& faceLocations);

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) const;

	~Skybox();

private:
	Mesh* SkyMesh{ };
	Shader* SkyShader{ };

	GLuint 
		TextureID{ },
		UniformProjection{ },
		UniformView{ };
};
