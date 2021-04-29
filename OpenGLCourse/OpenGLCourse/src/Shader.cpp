// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Shader.h"

Shader::Shader()
{
	shaderID = { };
	UniformModel = { };
	UniformProjection = { };

	pointLightCount = { };
	spotLightCount = { };
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content{ };
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line{ };
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Shader::CompileProgram() {

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	UniformProjection = glGetUniformLocation(shaderID, "projection");
	UniformModel = glGetUniformLocation(shaderID, "model");
	UniformView = glGetUniformLocation(shaderID, "view");
	UniformDirectionalLight.UniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	UniformDirectionalLight.UniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	UniformDirectionalLight.UniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	UniformDirectionalLight.UniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	UniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	UniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	UniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	UniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		UniformPointLight[i].UniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		UniformPointLight[i].UniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		UniformPointLight[i].UniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		UniformPointLight[i].UniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		UniformPointLight[i].UniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		UniformPointLight[i].UniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		UniformPointLight[i].UniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	UniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		UniformSpotLight[i].UniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		UniformSpotLight[i].UniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		UniformSpotLight[i].UniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		UniformSpotLight[i].UniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		UniformSpotLight[i].UniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		UniformSpotLight[i].UniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		UniformSpotLight[i].UniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		UniformSpotLight[i].UniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		UniformSpotLight[i].UniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	UniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	UniformTexture = glGetUniformLocation(shaderID, "theTexture");
	UniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	UniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	UniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (int i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		UniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}

	for (int i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		UniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		UniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}
}

GLuint Shader::GetProjectionLocation()
{
	return UniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return UniformModel;
}

GLuint Shader::GetViewLocation()
{
	return UniformView;
}

GLuint Shader::GetAmbientColourLocation()
{
	return UniformDirectionalLight.UniformColour;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return UniformDirectionalLight.UniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return UniformDirectionalLight.UniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return UniformDirectionalLight.UniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return UniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return UniformShininess;
}

GLuint Shader::GetEyePositionLocation()
{
	return UniformEyePosition;
}

GLuint Shader::GetOmniLightPosLocation()
{
	return UniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation()
{
	return UniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(UniformDirectionalLight.UniformAmbientIntensity, UniformDirectionalLight.UniformColour,
		UniformDirectionalLight.UniformDiffuseIntensity, UniformDirectionalLight.UniformDirection);
}

void Shader::SetPointLights(PointLight * pLight, int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(UniformPointLightCount, lightCount);

	for (int i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(UniformPointLight[i].UniformAmbientIntensity, UniformPointLight[i].UniformColour,
			UniformPointLight[i].UniformDiffuseIntensity, UniformPointLight[i].UniformPosition,
			UniformPointLight[i].UniformConstant, UniformPointLight[i].UniformLinear, UniformPointLight[i].UniformExponent);

		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(UniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(UniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(SpotLight * sLight, GLint lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(UniformSpotLightCount, lightCount);

	for (GLint i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(UniformSpotLight[i].UniformAmbientIntensity, UniformSpotLight[i].UniformColour,
			UniformSpotLight[i].UniformDiffuseIntensity, UniformSpotLight[i].UniformPosition, UniformSpotLight[i].UniformDirection,
			UniformSpotLight[i].UniformConstant, UniformSpotLight[i].UniformLinear, UniformSpotLight[i].UniformExponent,
			UniformSpotLight[i].UniformEdge);

		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(UniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(UniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(UniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(UniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 lTransform)
{
	glUniformMatrix4fv(UniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(UniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	UniformModel = 0;
	UniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	glShaderSource(theShader, 1, theCode, NULL);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}
