// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture() const;
	void ClearTexture();

	~Texture();

private:
	GLuint TextureID{ };

	GLint
		Width{ },
		Height{ },
		BitDepth{ };

	const GLchar* FileLocation{ };
};
