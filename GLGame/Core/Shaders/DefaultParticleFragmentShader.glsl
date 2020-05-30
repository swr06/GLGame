#version 330 core

in vec4 frag_color;
in vec2 tex_coords;

out vec4 color

void main() 
{
	color = frag_color;
}