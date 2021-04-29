// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Skybox.h"

Skybox::Skybox() { }

Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// Shader Setup
	SkyShader = new Shader();
	SkyShader->CreateFromFiles("Shaders/skybox_vert.glsl", "Shaders/skybox_frag.glsl");

	UniformProjection = SkyShader->GetProjectionLocation();
	UniformView = SkyShader->GetViewLocation();


	// Texture Setup
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

	int Width, Height, BitDepth;

	for (GLenum i = 0; i < 6; i++)
	{
		unsigned char *TexData = stbi_load(faceLocations[i].c_str(), &Width, &Height, &BitDepth, 0);
		if (!TexData)
		{
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TexData);
		stbi_image_free(TexData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Mesh Setup
	unsigned int SkyboxIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float SkyboxVertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	SkyMesh = new Mesh();
	SkyMesh->CreateMesh(SkyboxVertices, SkyboxIndices, 64, 36);
}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);

	SkyShader->UseShader();

	glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(UniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

	SkyShader->Validate();

	SkyMesh->RenderMesh();

	glDepthMask(GL_TRUE);
}

Skybox::~Skybox() { }
