#version 460

in vec3 TexCoords;

out vec4 colour;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, TexCoords);
}
