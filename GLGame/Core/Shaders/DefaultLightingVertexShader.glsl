#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 _color;
layout(location = 2) in vec2 uv_coords;

out vec2 UVCoords;
out vec4 FragColor;

uniform mat4 u_ViewProjectionMatrix = mat4(1.0f);

void main()
{
	FragColor = _color;
	UVCoords = uv_coords; 
	gl_Position = u_ViewProjectionMatrix * vec4(position, 1.0f); 
}