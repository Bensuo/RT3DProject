// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

#define NR_LIGHTS 4

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
};

uniform mat4 modelview;
uniform mat4 projection;

uniform lightStruct light[NR_LIGHTS];

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec3 ex_L[NR_LIGHTS];

out vec2 ex_TexCoord;

// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(position,1.0);

	// Find V - in eye coordinates, eye is at (0,0,0)
	FragPos = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	Normal = normalize(normalmatrix * normal);

	// L - to light source from vertex
	for(int i = 0; i < NR_LIGHTS; i++)
		ex_L[i] = normalize(light[i].position.xyz - vertexPosition.xyz);

	ex_TexCoord = texCoords;

    gl_Position = projection * vertexPosition;

}