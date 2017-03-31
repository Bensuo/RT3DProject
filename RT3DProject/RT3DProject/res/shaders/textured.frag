
#version 330
precision highp float;

uniform sampler2D textureUnit0;

in vec2 TexCoords;

layout(location = 0) out vec4 out_Color;
 
void main(void) 
{
	out_Color = texture(textureUnit0, TexCoords);
}