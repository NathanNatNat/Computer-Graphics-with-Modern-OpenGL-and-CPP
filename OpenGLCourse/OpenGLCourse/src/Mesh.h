// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, size_t numOfVertices, size_t numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO{ }, VBO{ }, IBO{ };
	GLsizei IndexCount{ };
};
