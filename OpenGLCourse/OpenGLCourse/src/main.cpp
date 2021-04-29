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
#include "CommonValues.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"

GLuint 
	UniformProjection = { 0 },
	UniformModel = { 0 },
	UniformView = { 0 },
	UniformEyePosition = { 0 },
	UniformSpecularIntensity = { 0 },
	UniformShininess = { 0 },
	UniformDirectionalLightTransform = { 0 },
	UniformOmniLightPos = { 0 },
	UniformFarPlane = { 0 };

Window MainWindow{ };

std::vector<Mesh*> MeshList{ };

std::vector<Shader> ShaderList{ };

Shader 
	DirectionalShadowShader{ },
	OmniShadowShader{ };

Camera camera;

Texture 
	BrickTexture{ },
	DirtTexture{ },
	PlainTexture{ };

Material 
	ShinyMaterial{ },
	DullMaterial{ };

Model 
	Xwing{ },
	Blackhawk{ };

DirectionalLight MainLight{ };
PointLight PointLights[MAX_POINT_LIGHTS]{ };
SpotLight SpotLights[MAX_SPOT_LIGHTS]{ };

Skybox skybox{ };

unsigned int 
	PointLightCount = { 0 },
	SpotLightCount = { 0 };

GLdouble 
	DeltaTime = { 0.0f },
	LastTime = { 0.0f };

GLfloat BlackhawkAngle = { 0.0f };

// Vertex Shader
static const char* vShader = { "Shaders/shader_vert.glsl" };

// Fragment Shader
static const char* fShader = { "Shaders/shader_frag.glsl" };

void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int 
			In0 = indices[i] * vLength,
			In1 = indices[i + 1] * vLength,
			In2 = indices[i + 2] * vLength;

		glm::vec3 V1(vertices[In1] - vertices[In0], vertices[In1 + 1] - vertices[In0 + 1], vertices[In1 + 2] - vertices[In0 + 2]);
		glm::vec3 V2(vertices[In2] - vertices[In0], vertices[In2 + 1] - vertices[In0 + 1], vertices[In2 + 2] - vertices[In0 + 2]);
		glm::vec3 Normal = { glm::cross(V1, V2) };
		Normal = { glm::normalize(Normal) };
		
		In0 += normalOffset; In1 += normalOffset; In2 += normalOffset;
		vertices[In0] += Normal.x; vertices[In0 + 1] += Normal.y; vertices[In0 + 2] += Normal.z;
		vertices[In1] += Normal.x; vertices[In1 + 1] += Normal.y; vertices[In1 + 2] += Normal.z;
		vertices[In2] += Normal.x; vertices[In2 + 1] += Normal.y; vertices[In2 + 2] += Normal.z;
	}

	for (GLuint i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 Vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		Vec = { glm::normalize(Vec) };
		vertices[nOffset] = Vec.x; vertices[nOffset + 1] = Vec.y; vertices[nOffset + 2] = Vec.z;
	}
}

void CreateObjects() 
{
	unsigned int Indices[] = 
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

	unsigned int FloorIndices[] = 
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat FloorVertices[] = 
	{
		-10.0f, 0.0f, -10.0f, 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f, 0.0f,  10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f, 10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(Indices, 12, Vertices, 32, 8, 5);

	Mesh* Obj1 = { new Mesh() };
	Obj1->CreateMesh(Vertices, Indices, 32, 12);
	MeshList.push_back(Obj1);

	Mesh* Obj2 = { new Mesh() };
	Obj2->CreateMesh(Vertices, Indices, 32, 12);
	MeshList.push_back(Obj2);

	Mesh* Obj3 = { new Mesh() };
	Obj3->CreateMesh(FloorVertices, FloorIndices, 32, 6);
	MeshList.push_back(Obj3);
}

void CreateShaders()
{
	Shader* Shader1 = { new Shader() };
	Shader1->CreateFromFiles(vShader, fShader);
	ShaderList.push_back(*Shader1);

	DirectionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	OmniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f);

	model = { glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)) };
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	BrickTexture.UseTexture();
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
	MeshList[0]->RenderMesh();

	model = { glm::mat4(1.0f) };
	model = { glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f)) };
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DirtTexture.UseTexture();
	DullMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
	MeshList[1]->RenderMesh();

	model = { glm::mat4(1.0f) };
	model = { glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)) };
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DirtTexture.UseTexture();
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
	MeshList[2]->RenderMesh();

	model = { glm::mat4(1.0f) };
	model = { glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f)) };
	model = { glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f)) };
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
	Xwing.RenderModel();

	BlackhawkAngle += 0.1f;
	if (BlackhawkAngle > 360.0f)
	{
		BlackhawkAngle = { 0.1f };
	}

	model = { glm::mat4(1.0f) };
	model = { glm::rotate(model, glm::radians(-BlackhawkAngle), glm::vec3(0.0f, 1.0f, 0.0f)) };
	model = { glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f)) };
	model = { glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f)) };
	model = { glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) };
	model = { glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)) };
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformShininess);
	Blackhawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	DirectionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModel = { DirectionalShadowShader.GetModelLocation() };
	DirectionalShadowShader.SetDirectionalLightTransform(light->CalculateLightTransform());

	DirectionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	OmniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModel = { OmniShadowShader.GetModelLocation() };
	UniformOmniLightPos = { OmniShadowShader.GetOmniLightPosLocation() };
	UniformFarPlane = { OmniShadowShader.GetFarPlaneLocation() };

	glUniform3f(UniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(UniformFarPlane, light->GetFarPlane());
	OmniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	OmniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	ShaderList[0].UseShader();

	UniformModel = { ShaderList[0].GetModelLocation() };
	UniformProjection = { ShaderList[0].GetProjectionLocation() };
	UniformView = { ShaderList[0].GetViewLocation() };
	UniformModel = { ShaderList[0].GetModelLocation() };
	UniformEyePosition = { ShaderList[0].GetEyePositionLocation() };
	UniformSpecularIntensity = { ShaderList[0].GetSpecularIntensityLocation() };
	UniformShininess = { ShaderList[0].GetShininessLocation() };

	glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(UniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(UniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	ShaderList[0].SetDirectionalLight(&MainLight);
	ShaderList[0].SetPointLights(PointLights, PointLightCount, 3, 0);
	ShaderList[0].SetSpotLights(SpotLights, SpotLightCount, 3 + PointLightCount, PointLightCount);
	ShaderList[0].SetDirectionalLightTransform(MainLight.CalculateLightTransform());

	MainLight.GetShadowMap()->Read(GL_TEXTURE2);
	ShaderList[0].SetTexture(1);
	ShaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 LowerLight = { camera.GetCameraPosition() };
	LowerLight.y -= 0.3f;
	SpotLights[0].SetFlash(LowerLight, camera.GetCameraDirection());

	ShaderList[0].Validate();

	RenderScene();
}

int main() 
{
	MainWindow = { Window(1366, 768) }; // 1280, 1024 or 1024, 768
	MainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = { Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f) };

	BrickTexture = { Texture("Textures/brick.png") };
	BrickTexture.LoadTextureA();
	DirtTexture = { Texture("Textures/dirt.png") };
	DirtTexture.LoadTextureA();
	PlainTexture = { Texture("Textures/plain.png") };
	PlainTexture.LoadTextureA();

	ShinyMaterial = { Material(4.0f, 256) };
	DullMaterial = { Material(0.3f, 4) };

	Xwing = { Model() };
	Xwing.LoadModel("Models/x-wing.obj");

	Blackhawk = { Model() };
	Blackhawk.LoadModel("Models/uh60.obj");

	MainLight = { DirectionalLight(2048, 2048, 1.0f, 0.53f, 0.3f, 0.1f, 0.9f, -10.0f, -12.0f, 18.5f) };

	PointLights[0] = { PointLight(1024, 1024, 0.01f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 2.0f, 0.0f, 0.3f, 0.2f, 0.1f) };

	PointLightCount++;

	PointLights[1] = { PointLight(1024, 1024, 0.01f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -4.0f, 3.0f, 0.0f, 0.3f, 0.2f, 0.1f) };

	PointLightCount++;

	SpotLights[0] = { SpotLight(1024, 1024, 0.01f, 100.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.0f) };

	SpotLightCount++;

	SpotLights[1] = { SpotLight(1024, 1024, 0.01f, 100.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.5f, 0.0f, -100.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.0f) };

	SpotLightCount++;

	std::vector<std::string> SkyboxFaces{ };
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	SkyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = { Skybox(SkyboxFaces) };

	GLuint 
		UniformProjection = { 0 },
		UniformModel = { 0 },
		UniformView = { 0 },
		UniformEyePosition = { 0 },
		UniformSpecularIntensity = { 0 },
		UniformShininess = { 0 };

	glm::mat4 Projection = { glm::perspective(glm::radians(60.0f), (GLfloat)MainWindow.GetBufferWidth() / MainWindow.GetBufferHeight(), 0.1f, 100.0f) };

	// Loop until window closed
	while (!MainWindow.GetShouldClose())
	{
		GLdouble Now = { glfwGetTime() };
		DeltaTime = { Now - LastTime };
		LastTime = { Now };

		// Get + Handle User Input
		glfwPollEvents();

		camera.KeyControl(MainWindow.GetKeys(), DeltaTime);
		camera.MouseControl(MainWindow.GetXChange(), MainWindow.GetYChange());

		if (MainWindow.GetKeys()[GLFW_KEY_L])
		{
			SpotLights[0].Toggle();
			MainWindow.GetKeys()[GLFW_KEY_L] = false;
		}

		DirectionalShadowMapPass(&MainLight);
		for (size_t i = 0; i < PointLightCount; i++)
		{
			OmniShadowMapPass(&PointLights[i]);
		}
		for (size_t i = 0; i < SpotLightCount; i++)
		{
			OmniShadowMapPass(&SpotLights[i]);
		}

		RenderPass(camera.CalculateViewMatrix(), Projection);

		MainWindow.SwapBuffers();
	}

	return 0;
}
