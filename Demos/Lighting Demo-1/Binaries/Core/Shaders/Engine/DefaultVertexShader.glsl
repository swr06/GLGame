#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec4 MultiplyColor;

uniform mat4 u_ModelMatrix = mat4(1.0f);
uniform mat4 u_ViewProjectionMatrix = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(position, 1.0f); 
	TexCoord = texCoord; 
	MultiplyColor = color;
}