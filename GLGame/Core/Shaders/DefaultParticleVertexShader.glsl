#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProj * u_Transform * vec4(Position, 1.0);
}
