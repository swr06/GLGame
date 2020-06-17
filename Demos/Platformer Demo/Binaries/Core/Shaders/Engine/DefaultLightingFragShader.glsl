#version 330 core
in vec2 UVCoords;
in vec4 FragColor;
out vec4 color;

void main()
{
	// finds the length of a pixel from 0,0 which controls the alpha
	float dist = length(UVCoords); 

	// Recommended formula for light as it follows a much more natural decent
    color = vec4(FragColor.r, FragColor.g, FragColor.b, FragColor.a * (pow(0.01, dist) - 0.01));

    // Straight lighting formula (Doesn't look that good)
    //color = vec4(FragColor.r, FragColor.g, FragColor.b, FragColor.a * (1.0 - dist));
}