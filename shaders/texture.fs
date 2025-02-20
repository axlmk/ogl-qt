#version 330 core

// Phong shader

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	vec3 position;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

in vec3 normal;
in vec3 fragPos;
in vec2 textureCoord;
out vec4 FragColor;

void main()
{
	// Ambient
	vec3 ambient	= light.ambient * vec3(texture(material.diffuse, textureCoord));

	// Diffuse
	vec3 lightDir	= normalize(light.position - fragPos);
	float diff		= max(dot(normal, lightDir), 0.0);
	vec3 diffuse	= light.diffuse * vec3(texture(material.diffuse, textureCoord)) * diff;

	// Specular
	vec3 cameraDir	= normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec		= pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
	vec3 specular	= light.specular * material.specular * spec;

	// Output
	vec3 outColor	= ambient + diffuse;
	FragColor = vec4(outColor, 1.0);
}
