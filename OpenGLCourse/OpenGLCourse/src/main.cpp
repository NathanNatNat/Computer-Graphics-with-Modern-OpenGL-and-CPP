// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

Window MainWindow{ };
std::vector<Mesh*> MeshList{ };
std::vector<Shader> ShaderList{ };
Camera camera{ };

Texture 
	BrickTexture{ },
	DirtTexture{ };

Material 
	ShinyMaterial{ },
	DullMaterial{ };

Light MainLight;

GLfloat 
	DeltaTime = { },
	LastTime = { };

// Vertex Shader
static const char* VShader = { "Shaders/shader_vert.glsl" };

// Fragment Shader
static const char* FShader = { "Shaders/shader_frag.glsl" };

void calcAverageNormals(const unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (unsigned int i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (unsigned int i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	GLuint Indices[] = 
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat Vertices[] = 
	{
	   // X      Y      Z     U     V     NX    NY    NZ
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	calcAverageNormals(Indices, 12, Vertices, 32, 8, 5);

	Mesh* Obj1 = new Mesh();
	Obj1->CreateMesh(Vertices, Indices, 32, 12);
	MeshList.push_back(Obj1);

	Mesh* Obj2 = new Mesh();
	Obj2->CreateMesh(Vertices, Indices, 32, 12);
	MeshList.push_back(Obj2);
}

void CreateShaders()
{
	Shader* Shader1 = new Shader();
	Shader1->CreateFromFiles(VShader, FShader);
	ShaderList.push_back(*Shader1);
}

int main()
{
	MainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	MainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	BrickTexture = Texture(const_cast<char*>("Textures/brick.png"));
	BrickTexture.LoadTexture();
	DirtTexture = Texture(const_cast<char*>("Textures/dirt.png"));
	DirtTexture.LoadTexture();

	ShinyMaterial = Material(1.0f, 32.0f);
	DullMaterial = Material(0.3f, 4.0f);

	MainLight = Light(1.0f, 1.0f, 1.0f, 0.1f, 0.0f, 0.0f, -1.0f, 0.3f);

	GLuint
		UniformProjection = { },
		UniformModel = { },
		UniformView = { },
		UniformEyePosition = { },
		UniformAmbientIntensity = { },
		UniformAmbientColour = { },
		UniformDirection = { },
		UniformDiffuseIntensity = { },
		UniformSpecularIntensity = { },
		UniformShininess = { };

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(MainWindow.GetBufferWidth()) / MainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!MainWindow.GetShouldClose())
	{
		GLfloat Now = static_cast<GLfloat>(glfwGetTime());
		DeltaTime = Now - LastTime;
		LastTime = Now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.KeyControl(MainWindow.GetKeys(), DeltaTime);
		//camera.MouseControl(MainWindow.GetXChange(), MainWindow.GetYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderList[0].UseShader();
		UniformModel = ShaderList[0].GetModelLocation();
		UniformProjection = ShaderList[0].GetProjectionLocation();
		UniformView = ShaderList[0].GetViewLocation();
		UniformAmbientColour = ShaderList[0].GetAmbientColourLocation();
		UniformAmbientIntensity = ShaderList[0].GetAmbientIntensityLocation();
		UniformDirection = ShaderList[0].GetDirectionLocation();
		UniformDiffuseIntensity = ShaderList[0].GetDiffuseIntensityLocation();
		UniformEyePosition = ShaderList[0].GetEyePositionLocation();
		UniformSpecularIntensity = ShaderList[0].GetSpecularIntensityLocation();
		UniformShininess = ShaderList[0].GetShininessLocation();

		MainLight.UseLight(UniformAmbientIntensity, UniformAmbientColour, UniformDiffuseIntensity, UniformDirection);

		glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(Projection));
		glUniformMatrix4fv(UniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		glm::vec3 CameraPosition = camera.GetCameraPosition();
		glUniform3f(UniformEyePosition, CameraPosition.x, CameraPosition.y, CameraPosition.z);

		glm::mat4 Model(1.0f);

		Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -2.5f));
		//Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(Model));
		BrickTexture.UseTexture();
		ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
		MeshList[0]->RenderMesh();

		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(0.0f, 4.0f, -2.5f));
		//Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(Model));
		DirtTexture.UseTexture();
		DullMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
		MeshList[1]->RenderMesh();

		glUseProgram(0);

		MainWindow.SwapBuffers();
	}

	return 0;
}
