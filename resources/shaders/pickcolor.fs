#version 330 core

out vec4 FragColor;

uniform vec3 desiredColor;

void main()
{
	FragColor = vec4(desiredColor, 1.0);
}
