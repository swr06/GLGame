#version 330 core

#define MAX_TEX_SLOTS 32
in vec2 TexCoord;

// I am using a batch rendering method. The "TexElement" is the slot to which the texture is bound to
in flat int TexElement;

// Color to multiply with the texture color also called tint
in flat vec4 MultiplyColor;

// Outputted color
out vec4 color;

uniform sampler2D u_Textures[MAX_TEX_SLOTS];

void main()
{
	switch (TexElement)
	{
		case 0 : color = texture(u_Textures[0], TexCoord) * MultiplyColor; break;
		case 1 : color = texture(u_Textures[1], TexCoord) * MultiplyColor; break;
		case 2 : color = texture(u_Textures[2], TexCoord) * MultiplyColor; break;
		case 3 : color = texture(u_Textures[3], TexCoord) * MultiplyColor; break;
		case 4 : color = texture(u_Textures[4], TexCoord) * MultiplyColor; break;
		case 5 : color = texture(u_Textures[5], TexCoord) * MultiplyColor; break;
		case 6 : color = texture(u_Textures[6], TexCoord) * MultiplyColor; break;
		case 7 : color = texture(u_Textures[7], TexCoord) * MultiplyColor; break;
		case 8 : color = texture(u_Textures[8], TexCoord) * MultiplyColor; break;
		case 9 : color = texture(u_Textures[9], TexCoord) * MultiplyColor; break;
		case 10 : color = texture(u_Textures[10], TexCoord) * MultiplyColor; break;
		case 11 : color = texture(u_Textures[11], TexCoord) * MultiplyColor; break;
		case 12 : color = texture(u_Textures[12], TexCoord) * MultiplyColor; break;
		case 13 : color = texture(u_Textures[13], TexCoord) * MultiplyColor; break;
		case 14 : color = texture(u_Textures[14], TexCoord) * MultiplyColor; break;
		case 15 : color = texture(u_Textures[15], TexCoord) * MultiplyColor; break;
		case 16 : color = texture(u_Textures[16], TexCoord) * MultiplyColor; break;
		case 17 : color = texture(u_Textures[17], TexCoord) * MultiplyColor; break;
		case 18 : color = texture(u_Textures[18], TexCoord) * MultiplyColor; break;
		case 19 : color = texture(u_Textures[19], TexCoord) * MultiplyColor; break;
		case 20 : color = texture(u_Textures[20], TexCoord) * MultiplyColor; break;
		case 21 : color = texture(u_Textures[21], TexCoord) * MultiplyColor; break;
		case 22 : color = texture(u_Textures[22], TexCoord) * MultiplyColor; break;
		case 23 : color = texture(u_Textures[23], TexCoord) * MultiplyColor; break;
		case 24 : color = texture(u_Textures[24], TexCoord) * MultiplyColor; break;
		case 25 : color = texture(u_Textures[25], TexCoord) * MultiplyColor; break;
		case 26 : color = texture(u_Textures[26], TexCoord) * MultiplyColor; break;
		case 27 : color = texture(u_Textures[27], TexCoord) * MultiplyColor; break;
		case 28 : color = texture(u_Textures[28], TexCoord) * MultiplyColor; break;
		case 29 : color = texture(u_Textures[29], TexCoord) * MultiplyColor; break;
		case 30 : color = texture(u_Textures[30], TexCoord) * MultiplyColor; break;
		case 31 : color = texture(u_Textures[31], TexCoord) * MultiplyColor; break;
		default : color = MultiplyColor; break;
	}
}