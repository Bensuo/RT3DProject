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
uniform float interp;
uniform lightStruct light[NR_LIGHTS];

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec3 ex_L[NR_LIGHTS];
out vec2 ex_TexCoord;

in vec3 in_Position_next;
// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {
	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(mix(in_Position, in_Position_next, interp),1.0);

	// Find V - in eye coordinates, eye is at (0,0,0)
	FragPos = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	Normal = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	for(int i = 0; i < NR_LIGHTS; i++)
		ex_L[i] = normalize(light[i].position.xyz - vertexPosition.xyz);

	ex_TexCoord = in_TexCoord;

    gl_Position = projection * vertexPosition;

}