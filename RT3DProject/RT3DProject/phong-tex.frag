// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

#define NR_LIGHTS 4

// Some drivers require the following
precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform lightStruct light[NR_LIGHTS];
uniform materialStruct material;
uniform sampler2D textureUnit0;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L[NR_LIGHTS];
in vec2 ex_TexCoord;
layout(location = 0) out vec4 out_Color;

vec4 CalcColor(lightStruct newLight, vec3 L);
 
void main(void) 
{
    for(int i = 0; i < NR_LIGHTS; i++)
		out_Color += CalcColor(light[i], ex_L[i]);
}

vec4 CalcColor(lightStruct newLight, vec3 L)
{
	// Ambient intensity
	vec4 ambientI = newLight.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = newLight.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-L),normalize(ex_N)));

	vec4 specularI = newLight.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	// Fragment colour
	return vec4(ambientI + diffuseI + specularI) * texture(textureUnit0, ex_TexCoord);
	//out_Color = texture2D(textureUnit0, ex_TexCoord);
}