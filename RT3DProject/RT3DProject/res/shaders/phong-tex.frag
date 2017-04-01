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
	// Ambient
	vec4 ambient = light.ambient * texture(textureUnit0, TexCoords);

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position.xyz - FragPos.xyz);
	float diff = max(dot(norm, lightDir),0);
	vec4 diffuse = light.diffuse * diff * texture(textureUnit0, TexCoords);
	
	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);
	vec4 specular = light.specular * spec * texture(textureUnit0, TexCoords);

	return vec4(ambient + diffuse + specular);
}