#include "Shader.h"
#include <iostream>

Shader::Shader()
{
	ShaderID = { };
	UniformModel = { };
	UniformProjection = { };
	PointLightCount = { };
	SpotLightCount = { };
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	const std::string VertexString = { ReadFile(vertexLocation) };
	const std::string FragmentString = { ReadFile(fragmentLocation) };
	const char* VertexCode = { VertexString.c_str() };
	const char* FragmentCode = { FragmentString.c_str() };

	CompileShader(VertexCode, FragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	const std::string VertexString = { ReadFile(vertexLocation) };
	const std::string GeometryString = { ReadFile(geometryLocation) };
	const std::string FragmentString = { ReadFile(fragmentLocation) };
	const char* VertexCode = { VertexString.c_str() };
	const char* GeometryCode = { GeometryString.c_str() };
	const char* FragmentCode = { FragmentString.c_str() };

	CompileShader(VertexCode, GeometryCode, FragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string Content{ };
	std::ifstream FileStream(fileLocation, std::ios::in);

	if (!FileStream.is_open()) 
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return { };
	}

	std::string Line{ };
	while (!FileStream.eof())
	{
		std::getline(FileStream, Line);
		Content.append(Line + "\n");
	}

	FileStream.close();
	return Content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	ShaderID = { glCreateProgram() };

	if (!ShaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(ShaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(ShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	ShaderID = { glCreateProgram() };

	if (!ShaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(ShaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(ShaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(ShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::Validate() const
{
	GLint Result{ };
	
	glValidateProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_VALIDATE_STATUS, &Result);
	if (!Result)
	{
		GLchar eLog[1024]{ };
		glGetProgramInfoLog(ShaderID, sizeof(eLog), nullptr, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Shader::CompileProgram() {

	GLint Result{ };

	glLinkProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &Result);
	if (!Result)
	{
		GLchar eLog[1024]{ };
		glGetProgramInfoLog(ShaderID, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	UniformProjection = glGetUniformLocation(ShaderID, "projection");
	UniformModel = glGetUniformLocation(ShaderID, "model");
	UniformView = glGetUniformLocation(ShaderID, "view");
	UniformDirectionalLight.UniformColour = glGetUniformLocation(ShaderID, "directionalLight.base.colour");
	UniformDirectionalLight.UniformAmbientIntensity = glGetUniformLocation(ShaderID, "directionalLight.base.ambientIntensity");
	UniformDirectionalLight.UniformDirection = glGetUniformLocation(ShaderID, "directionalLight.direction");
	UniformDirectionalLight.UniformDiffuseIntensity = glGetUniformLocation(ShaderID, "directionalLight.base.diffuseIntensity");
	UniformSpecularIntensity = glGetUniformLocation(ShaderID, "material.specularIntensity");
	UniformShininess = glGetUniformLocation(ShaderID, "material.shininess");
	UniformEyePosition = glGetUniformLocation(ShaderID, "eyePosition");

	UniformPointLightCount = glGetUniformLocation(ShaderID, "pointLightCount");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char LocBuff[100] = {};

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].base.colour", i);
		UniformPointLight[i].UniformColour = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].base.ambientIntensity", i);
		UniformPointLight[i].UniformAmbientIntensity = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].base.diffuseIntensity", i);
		UniformPointLight[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].position", i);
		UniformPointLight[i].UniformPosition = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].constant", i);
		UniformPointLight[i].UniformConstant = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].linear", i);
		UniformPointLight[i].UniformLinear = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "pointLights[%d].exponent", i);
		UniformPointLight[i].UniformExponent = glGetUniformLocation(ShaderID, LocBuff);
	}

	UniformSpotLightCount = glGetUniformLocation(ShaderID, "spotLightCount");

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char LocBuff[100] = {};

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.base.colour", i);
		UniformSpotLight[i].UniformColour = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.base.ambientIntensity", i);
		UniformSpotLight[i].UniformAmbientIntensity = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		UniformSpotLight[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.position", i);
		UniformSpotLight[i].UniformPosition = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.constant", i);
		UniformSpotLight[i].UniformConstant = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.linear", i);
		UniformSpotLight[i].UniformLinear = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].base.exponent", i);
		UniformSpotLight[i].UniformExponent = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].direction", i);
		UniformSpotLight[i].UniformDirection = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "spotLights[%d].edge", i);
		UniformSpotLight[i].UniformEdge = glGetUniformLocation(ShaderID, LocBuff);
	}

	UniformDirectionalLightTransform = glGetUniformLocation(ShaderID, "directionalLightTransform");
	UniformTexture = glGetUniformLocation(ShaderID, "theTexture");
	UniformDirectionalShadowMap = glGetUniformLocation(ShaderID, "directionalShadowMap");

	UniformOmniLightPos = glGetUniformLocation(ShaderID, "lightPos");
	UniformFarPlane = glGetUniformLocation(ShaderID, "farPlane");

	for (int i = 0; i < 6; i++)
	{
		char LocBuff[100] = {};

		snprintf(LocBuff, sizeof(LocBuff), "lightMatrices[%d]", i);
		UniformLightMatrices[i] = glGetUniformLocation(ShaderID, LocBuff);
	}

	for (int i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char LocBuff[100] = {};

		snprintf(LocBuff, sizeof(LocBuff), "omniShadowMaps[%d].shadowMap", i);
		UniformOmniShadowMap[i].shadowMap = glGetUniformLocation(ShaderID, LocBuff);

		snprintf(LocBuff, sizeof(LocBuff), "omniShadowMaps[%d].farPlane", i);
		UniformOmniShadowMap[i].FarPlane = glGetUniformLocation(ShaderID, LocBuff);
	}
}

GLuint Shader::GetProjectionLocation() const
{
	return UniformProjection;
}

GLuint Shader::GetModelLocation() const
{
	return UniformModel;
}

GLuint Shader::GetViewLocation() const
{
	return UniformView;
}

GLuint Shader::GetAmbientColourLocation() const
{
	return UniformDirectionalLight.UniformColour;
}

GLuint Shader::GetAmbientIntensityLocation() const
{
	return UniformDirectionalLight.UniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation() const
{
	return UniformDirectionalLight.UniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() const
{
	return UniformDirectionalLight.UniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation() const
{
	return UniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation() const
{
	return UniformShininess;
}

GLuint Shader::GetEyePositionLocation() const
{
	return UniformEyePosition;
}

GLuint Shader::GetOmniLightPosLocation() const
{
	return UniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation() const
{
	return UniformFarPlane;
}

void Shader::SetDirectionalLight(const DirectionalLight * dLight) const
{
	dLight->UseLight(UniformDirectionalLight.UniformAmbientIntensity, UniformDirectionalLight.UniformColour,
		UniformDirectionalLight.UniformDiffuseIntensity, UniformDirectionalLight.UniformDirection);
}

void Shader::SetPointLights(const PointLight * pLight, int lightCount, const unsigned int textureUnit, const unsigned int offset) const
{
	if (lightCount > MAX_POINT_LIGHTS) 
		lightCount = { MAX_POINT_LIGHTS };

	glUniform1i(UniformPointLightCount, lightCount);

	for (int i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(UniformPointLight[i].UniformAmbientIntensity, UniformPointLight[i].UniformColour,
			UniformPointLight[i].UniformDiffuseIntensity, UniformPointLight[i].UniformPosition,
			UniformPointLight[i].UniformConstant, UniformPointLight[i].UniformLinear, UniformPointLight[i].UniformExponent);

		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(UniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(UniformOmniShadowMap[i + offset].FarPlane, pLight[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(const SpotLight * sLight, GLint lightCount, const unsigned int textureUnit, const unsigned int offset) const
{
	if (lightCount > MAX_SPOT_LIGHTS) 
		lightCount = { MAX_SPOT_LIGHTS };

	glUniform1i(UniformSpotLightCount, lightCount);

	for (GLint i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(UniformSpotLight[i].UniformAmbientIntensity, UniformSpotLight[i].UniformColour,
			UniformSpotLight[i].UniformDiffuseIntensity, UniformSpotLight[i].UniformPosition, UniformSpotLight[i].UniformDirection,
			UniformSpotLight[i].UniformConstant, UniformSpotLight[i].UniformLinear, UniformSpotLight[i].UniformExponent,
			UniformSpotLight[i].UniformEdge);

		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(UniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(UniformOmniShadowMap[i + offset].FarPlane, sLight[i].GetFarPlane());
	}
}

void Shader::SetTexture(const GLuint textureUnit) const
{
	glUniform1i(UniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(const GLuint textureUnit) const
{
	glUniform1i(UniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 lTransform) const
{
	glUniformMatrix4fv(UniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) const
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(UniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader() const
{
	glUseProgram(ShaderID);
}

void Shader::ClearShader()
{
	if (ShaderID != 0)
	{
		glDeleteProgram(ShaderID);
		ShaderID = { };
	}

	UniformModel = { };
	UniformProjection = { };
}

void Shader::AddShader(const GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	const GLuint TheShader = { glCreateShader(shaderType) };

	const GLchar* TheCode[1]{ };
	TheCode[0] = { shaderCode };

	glShaderSource(TheShader, 1, TheCode, nullptr);
	glCompileShader(TheShader);

	GLint Result = { };

	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &Result);
	if (!Result)
	{
		GLchar eLog[1024] = { };
		glGetShaderInfoLog(TheShader, sizeof(eLog), nullptr, eLog);
		std::cout << "Error compiling the " << shaderType << " shader: " << eLog;
		return;
	}

	glAttachShader(theProgram, TheShader);
}

Shader::~Shader()
{
	ClearShader();
}
