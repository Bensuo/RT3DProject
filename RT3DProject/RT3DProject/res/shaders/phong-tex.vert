#version 330

uniform mat4 modelview;
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main(void) 
{
	FragPos = vec3(modelview * vec4(position,1.0));
    Normal = mat3(transpose(inverse(modelview))) * normal;  
	TexCoords = texCoords;
    gl_Position = projection * vec4(FragPos, 1.0);
}