// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Shader.h"

Shader::Shader()
{
	ShaderID = { };
	UniformModel = { };
	UniformProjection = { };
	PointLightCount = { };
}

void Shader::CreateFromString(const char* VertexCode, const char* FragmentCode)
{
	CompileShader(VertexCode, FragmentCode);
}

void Shader::CreateFromFiles(const char* VertexLocation, const char* FragmentLocation)
{
	std::string VertexString = ReadFile(VertexLocation);
	std::string FragmentString = ReadFile(FragmentLocation);
	const char* VertexCode = VertexString.c_str();
	const char* FragmentCode = FragmentString.c_str();

	CompileShader(VertexCode, FragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* VertexCode, const char* FragmentCode)
{
	ShaderID = glCreateProgram();

	if (!ShaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(ShaderID, VertexCode, GL_VERTEX_SHADER);
	AddShader(ShaderID, FragmentCode, GL_FRAGMENT_SHADER);

	GLint Result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &Result);
	if (!Result)
	{
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_VALIDATE_STATUS, &Result);
	if (!Result)
	{
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
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
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		UniformPointLight[i].UniformColour = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		UniformPointLight[i].UniformAmbientIntensity = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		UniformPointLight[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		UniformPointLight[i].UniformPosition = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		UniformPointLight[i].UniformConstant = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		UniformPointLight[i].UniformLinear = glGetUniformLocation(ShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		UniformPointLight[i].UniformExponent = glGetUniformLocation(ShaderID, locBuff);
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

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(UniformDirectionalLight.UniformAmbientIntensity, UniformDirectionalLight.UniformColour,
		UniformDirectionalLight.UniformDiffuseIntensity, UniformDirectionalLight.UniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(UniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(UniformPointLight[i].UniformAmbientIntensity, UniformPointLight[i].UniformColour,
			UniformPointLight[i].UniformDiffuseIntensity, UniformPointLight[i].UniformPosition,
			UniformPointLight[i].UniformConstant, UniformPointLight[i].UniformLinear, UniformPointLight[i].UniformExponent);
	}
}

void Shader::UseShader()
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


void Shader::AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType)
{
	GLuint TheShader = glCreateShader(ShaderType);

	const GLchar* TheCode[1];
	TheCode[0] = ShaderCode;

	glShaderSource(TheShader, 1, TheCode, NULL);
	glCompileShader(TheShader);

	GLint Result = 0;

	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &Result);
	if (!Result)
	{
		GLchar eLog[1024] = { 0 };
		glGetShaderInfoLog(TheShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", static_cast<GLint>(ShaderType), eLog);
		return;
	}

	glAttachShader(TheProgram, TheShader);
}

Shader::~Shader()
{
	ClearShader();
}
