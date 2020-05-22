#version 330 core
in vec2 TexCoord;
in flat int TexElement;
in flat vec4 TexColor;
out vec4 color;

uniform sampler2D u_Textures[32];

void main()
{
	color = TexColor * texture(u_Textures[TexElement], TexCoord);
}