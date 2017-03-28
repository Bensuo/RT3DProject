#version 330

in vec2 in_TexCoord;
out vec2 ex_TexCoord;

void main(void) 
{
	ex_TexCoord = in_TexCoord;
}