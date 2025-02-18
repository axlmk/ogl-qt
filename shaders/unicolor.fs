#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;

void main()
{
	float ambientStrenght = 0.2;
	vec3 ambientLight = lightColor * ambientStrenght;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	vec3 outColor = objColor * (ambientLight + diffuse);
	FragColor = vec4(outColor, 1.0);
}
