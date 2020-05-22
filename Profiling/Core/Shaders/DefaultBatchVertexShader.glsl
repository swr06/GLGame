#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texOpacity;
layout(location = 3) in float texElement;

out vec2 TexCoord;
out int TexElement;
out float TexOpacity;

uniform mat4 u_ViewProjectionMatrix = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(position, 1.0f); 
	TexOpacity = texOpacity;
	TexCoord = texCoord; 
	TexElement = int(texElement);
}