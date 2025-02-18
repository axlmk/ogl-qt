#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 normal;
in vec3 fragPos;

void main()
{
	float ambientStrenght = 0.2;
	vec3 ambientLight = lightColor * ambientStrenght;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	vec3 cameraDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor;

	vec3 outColor = objColor * (ambientLight + diffuse + specular);
	FragColor = vec4(outColor, 1.0);
}
