// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <GL\glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* FileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint TextureID{ };

	GLint 
		Width{ },
		Height{ },
		BitDepth{ };

	char* FileLocation{ };
};
