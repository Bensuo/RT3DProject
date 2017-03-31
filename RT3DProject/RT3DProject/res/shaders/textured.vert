#version 330

uniform mat4 modelview;
uniform mat4 projection;

in vec3 position;
in vec2 texCoords;

out vec2 TexCoords;

void main(void) 
{
	vec4 vertexPosition = modelview * vec4(position,1.0);
    gl_Position = projection * vertexPosition;

	TexCoords = texCoords;
}