// The MAX_TEX_SLOTS macro and the version will be added internally while compiling the shader
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
		#if MAX_TEX_SLOTS > -1
			case 0 : color = texture(u_Textures[0], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 0
			case 1 : color = texture(u_Textures[1], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 1
			case 2 : color = texture(u_Textures[2], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 2
			case 3 : color = texture(u_Textures[3], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 3
			case 4 : color = texture(u_Textures[4], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 4
			case 5 : color = texture(u_Textures[5], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 5
			case 6 : color = texture(u_Textures[6], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 6
			case 7 : color = texture(u_Textures[7], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 7
			case 8 : color = texture(u_Textures[8], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 8
			case 9 : color = texture(u_Textures[9], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 9
			case 10 : color = texture(u_Textures[10], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 10
			case 11 : color = texture(u_Textures[11], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 11
			case 12 : color = texture(u_Textures[12], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 12
			case 13 : color = texture(u_Textures[13], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 13
			case 14 : color = texture(u_Textures[14], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 14
			case 15 : color = texture(u_Textures[15], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 15
			case 16 : color = texture(u_Textures[16], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 16
			case 17 : color = texture(u_Textures[17], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 17
			case 18 : color = texture(u_Textures[18], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 18
			case 19 : color = texture(u_Textures[19], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 19
			case 20 : color = texture(u_Textures[20], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 20
			case 21 : color = texture(u_Textures[21], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 21
			case 22 : color = texture(u_Textures[22], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 22
			case 23 : color = texture(u_Textures[23], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 23
			case 24 : color = texture(u_Textures[24], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 24
			case 25 : color = texture(u_Textures[25], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 25
			case 26 : color = texture(u_Textures[26], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 26
			case 27 : color = texture(u_Textures[27], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 27
			case 28 : color = texture(u_Textures[28], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 28
			case 29 : color = texture(u_Textures[29], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 29
			case 30 : color = texture(u_Textures[30], TexCoord) * MultiplyColor; break;
		#endif

		#if MAX_TEX_SLOTS > 30
			case 31 : color = texture(u_Textures[31], TexCoord) * MultiplyColor; break;
		#endif

		default : color = MultiplyColor; break;
	}
}