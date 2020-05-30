#version 330 core

layout (location = 1) in vec3 Position;
layout (location = 2) in vec4 Color;
layout (location = 3) in vec2 TexCoords;

out vec4 frag_color;
out vec2 tex_coords;

void main()
{
	gl_Position = Position;
	frag_color = Color;
	tex_coords = TexCoords;
}