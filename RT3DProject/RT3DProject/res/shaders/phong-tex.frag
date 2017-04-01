#version 330

#define NR_LIGHTS 4

precision highp float;

struct PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform PointLight lights[NR_LIGHTS];
uniform Material material;
uniform sampler2D textureUnit0;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

layout(location = 0) out vec4 out_Color;

vec4 CalcColor(PointLight light);
 
void main(void) 
{
    for(int i = 0; i < NR_LIGHTS; i++)
		out_Color += CalcColor(lights[i]);
}

vec4 CalcColor(PointLight light)
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

	return vec4(ambientI + diffuseI + specularI) * texture(textureUnit0, TexCoords);
}