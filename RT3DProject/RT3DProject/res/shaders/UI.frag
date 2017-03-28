#version 330

uniform vec4 colour;
uniform sampler2D textureUnit;

in vec2 ex_TexCoord;
layout(location = 0) out vec4 out_Color;
 
void main(void) 
{
	out_Color = vec4(colour * texture(textureUnit, ex_TexCoord));
}