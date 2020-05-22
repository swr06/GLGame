#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 texColor;
layout(location = 3) in float texElement;

out vec2 TexCoord;
out int TexElement;
out vec4 TexColor;

uniform mat4 u_ViewProjectionMatrix = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(position, 1.0f); 
	TexColor = texColor;
	TexCoord = texCoord; 
	TexElement = int(texElement);
}