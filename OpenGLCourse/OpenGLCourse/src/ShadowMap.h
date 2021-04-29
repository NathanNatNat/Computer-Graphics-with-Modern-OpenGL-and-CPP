// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <stdio.h>
#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	virtual void Write();

	virtual void Read(GLenum TextureUnit);

	GLuint GetShadowWidth() { return ShadowWidth; }
	GLuint GetShadowHeight() { return ShadowHeight; }

	~ShadowMap();
protected:
	GLuint 
		FBO{ },
		shadowMap{ },
		ShadowWidth{ },
		ShadowHeight{ };
};
