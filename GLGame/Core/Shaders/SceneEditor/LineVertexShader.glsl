#version 330 core

layout (location = 0) in vec2 Position;

void main()
{
	gl_PointSize = 2.0f;
	gl_Position = Position;
}