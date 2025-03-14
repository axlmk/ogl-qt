#version 330 core

// Phong shader

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	//vec3 direction;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;

	float linear;
	float quadratic;
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
	vec3 direction = light.position - fragPos;

	// Ambient
	vec3 ambient	= light.ambient * vec3(texture(material.diffuse, textureCoord));

	// Diffuse
	vec3 lightDir	= normalize(direction);
	float diff		= max(dot(normal, lightDir), 0.0);
	vec3 diffuse	= light.diffuse * vec3(texture(material.diffuse, textureCoord)) * diff;

	// Specular
	vec3 cameraDir	= normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec		= pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
	vec3 specular	= light.specular * spec * vec3(texture(material.specular, textureCoord));

	// attenuation
	float distance	= length(direction);
	float attenuation = 1.0 / ( 1.0 + light.linear * distance + light.quadratic * distance * distance );

	// Output
	vec3 outColor	= (ambient + diffuse + specular) * 4 * attenuation;
	FragColor = vec4(outColor, 1.0);
}
