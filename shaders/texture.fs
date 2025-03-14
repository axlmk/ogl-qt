#version 330 core

// Phong shader

struct Material {
	sampler2D	diffuse;
	sampler2D	specular;
	float		shininess;
};

struct Light {
	vec3	diffuse;
	vec3	specular;
	vec3	ambient;

	vec3	position;
	float	linear;
	float	quadratic;
	vec3	direction;
	float	cutoff;
	float	outerCutoff;
};

uniform vec3		cameraPos;
uniform Material	material;
uniform Light		light;

in vec3		normal;
in vec3		fragPos;
in vec2		textureCoord;

out vec4	FragColor;

void main()
{
	vec3 fragToLight	= normalize(light.position - fragPos);

	// Ambient
	vec3 ambient		= light.ambient * vec3(texture(material.diffuse, textureCoord));

	// Diffuse
	float diff			= max(dot(normal, fragToLight), 0.0);
	vec3 diffuse		= light.diffuse * vec3(texture(material.diffuse, textureCoord)) * diff;

	// Specular
	vec3 fragToCam		= normalize(cameraPos - fragPos);
	vec3 reflectDir		= reflect(-fragToLight, normal);
	float spec			= pow(max(dot(fragToCam, reflectDir), 0.0), material.shininess);
	vec3 specular		= light.specular * spec * vec3(texture(material.specular, textureCoord));

	// Cutoff
	float theta			= dot(fragToLight, normalize(-light.direction));
	float epsilon		= light.cutoff - light.outerCutoff;
	float intensity		= clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	// Output
	vec3 outCol		= ambient * 2 + (diffuse + specular) * 2 * intensity;
	FragColor		= vec4(outCol, 1.0);

	// attenuation
	//float distance	= length(lightDir);
	//float attenuation = 1.0 / ( 1.0 + light.linear * distance + light.quadratic * distance * distance );

}
