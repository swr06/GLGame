#version 330 core
in vec2 TexCoord;
in flat int TexElement;
out vec4 color;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[TexElement], TexCoord);
	//color = vec4(1.0f, 1.0f, 1.0f, 0.3f);
}