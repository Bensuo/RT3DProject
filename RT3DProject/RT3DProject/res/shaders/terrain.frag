#version 330

#define NR_LIGHTS 4

precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform lightStruct lights[NR_LIGHTS];
uniform materialStruct material;
uniform sampler2D bgTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendTexture;
uniform float rows;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

layout(location = 0) out vec4 out_Color;

vec4 CalcColor(lightStruct light, vec4 texColour);
 
void main(void) 
{
	vec2 blendCoords = TexCoords /rows;
	vec4 blendMapColour = texture(blendTexture, blendCoords);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec4 bgTextureColour = texture(bgTexture, TexCoords) * backTextureAmount;
	vec4 rTextureColour = texture(rTexture, TexCoords) * blendMapColour.r;
	vec4 gTextureColour = texture(gTexture, TexCoords) * blendMapColour.g;
	vec4 bTextureColour = texture(bTexture, TexCoords) * blendMapColour.b;
	vec4 totalColour = bgTextureColour + rTextureColour + gTextureColour + bTextureColour;
    for(int i = 0; i < NR_LIGHTS; i++)
		out_Color += CalcColor(lights[i], totalColour);
}

vec4 CalcColor(lightStruct light, vec4 texColour)
{
	vec3 lightDir = normalize(light.position.xyz - FragPos.xyz);

	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(Normal),normalize(lightDir)),0);

	// Specular intensity
	vec3 reflectDir = reflect(normalize(-lightDir),normalize(Normal));

	vec3 viewDir = normalize(-FragPos);
	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(reflectDir,viewDir),0), material.shininess);

	return vec4(ambientI + diffuseI + specularI) * texColour;
}