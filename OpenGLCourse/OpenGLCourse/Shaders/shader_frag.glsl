#version 460

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

out vec4 colour;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec3 ambientColour = directionalLight.colour * directionalLight.ambientIntensity;

	float diffuseFactor = dot(normalize(Normal), normalize(directionalLight.direction));

	diffuseFactor = max(diffuseFactor, 0.0f);

	vec3 diffuseColour = directionalLight.colour * directionalLight.diffuseIntensity * diffuseFactor;

	colour = texture(theTexture, TexCoord) * vec4((ambientColour + diffuseColour), 1.0f);
}
