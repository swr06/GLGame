#version 330 core

layout (location = 0) in vec2 Position;

void main()
{
	gl_Position = vec4(Position, 1.0f, 1.0f);
}