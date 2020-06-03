#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 FragColor;

out vec4 frag_color;

uniform mat4 u_ViewProjectionMatrix = mat4(1.0f);

void main()
{
	frag_color = FragColor;
	gl_Position = u_ViewProjectionMatrix * vec4(Position, 1.0);
}
