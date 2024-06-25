#pragma once
#include "ShadowMap.h"

class OmniShadowMap final : public ShadowMap
{
public:
	OmniShadowMap();

	bool Init(GLuint width, GLuint height) override;

	void Write() override;

	void Read(GLenum TextureUnit) override;

	~OmniShadowMap() override;
};
