// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	void Validate() const;

	static std::string ReadFile(const char* fileLocation);

	[[nodiscard]] GLuint GetProjectionLocation() const;
	[[nodiscard]] GLuint GetModelLocation() const;
	[[nodiscard]] GLuint GetViewLocation() const;
	[[nodiscard]] GLuint GetAmbientIntensityLocation() const;
	[[nodiscard]] GLuint GetAmbientColourLocation() const;
	[[nodiscard]] GLuint GetDiffuseIntensityLocation() const;
	[[nodiscard]] GLuint GetDirectionLocation() const;
	[[nodiscard]] GLuint GetSpecularIntensityLocation() const;
	[[nodiscard]] GLuint GetShininessLocation() const;
	[[nodiscard]] GLuint GetEyePositionLocation() const;
	[[nodiscard]] GLuint GetOmniLightPosLocation() const;
	[[nodiscard]] GLuint GetFarPlaneLocation() const;

	void SetDirectionalLight(const DirectionalLight * dLight) const;
	void SetPointLights(const PointLight * pLight, int lightCount, unsigned int textureUnit, unsigned int offset) const;
	void SetSpotLights(const SpotLight * sLight, GLint lightCount, unsigned int textureUnit, unsigned int offset) const;
	void SetTexture(GLuint textureUnit) const;
	void SetDirectionalShadowMap(GLuint textureUnit) const;
	void SetDirectionalLightTransform(glm::mat4 lTransform) const;
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices) const;

	void UseShader() const;
	void ClearShader();

	~Shader();

private:
	GLint
		PointLightCount{ },
		SpotLightCount{ };

	GLuint
		ShaderID{ },
		UniformProjection{ },
		UniformModel{ },
		UniformView{ },
		UniformEyePosition{ },
		UniformSpecularIntensity{ },
		UniformShininess{ },
		UniformTexture{ },
		UniformDirectionalShadowMap{ },
		UniformDirectionalLightTransform{ },
		UniformOmniLightPos{ },
		UniformFarPlane{ },
		UniformLightMatrices[6]{ };

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
			UniformColour{ },
			UniformAmbientIntensity{ },
			UniformDiffuseIntensity{ },
			UniformPosition{ },
			UniformConstant{ },
			UniformLinear{ },
			UniformExponent{ };

	} UniformPointLight[MAX_POINT_LIGHTS]{ };

	GLuint UniformSpotLightCount{ };

	struct
	{
		GLuint 
			UniformColour{ },
			UniformAmbientIntensity{ },
			UniformDiffuseIntensity{ },
			UniformPosition{ },
			UniformConstant{ },
			UniformLinear{ },
			UniformExponent{ },
			UniformDirection{ },
			UniformEdge{ };

	} UniformSpotLight[MAX_SPOT_LIGHTS]{ };

	struct
	{
		GLuint
			shadowMap{ },
			FarPlane{ };

	} UniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS]{ };

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	static void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};
