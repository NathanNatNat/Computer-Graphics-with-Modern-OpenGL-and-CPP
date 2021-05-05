// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Material.h"

Material::Material()
{
	SpecularIntensity = { };
	Shininess = { };
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	SpecularIntensity = { sIntensity };
	Shininess = { shine };
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, SpecularIntensity);
	glUniform1f(shininessLocation, Shininess);
}

Material::~Material() { }
