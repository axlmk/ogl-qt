#version 330 core

// Phong shader

#define MAX_LIGHTS 10

#define LIGHT_SPOT			0
#define LIGHT_POINT			1
#define LIGHT_DIRECTIONAL	2

struct Material
{
	sampler2D	diffuse_1;
	sampler2D	specular_1;
	float		shininess;
};

struct Light
{
	vec3	diffuse;
	vec3	specular;
	vec3	ambient;

	int		type;
	vec3	position;
	vec3	direction;
	
	float	linear;
	float	quadratic;
	float	cutoff;
	float	outerCutoff;
};

uniform vec3		cameraPos;
uniform Material	material;
uniform int			nLights;
uniform Light		lights[MAX_LIGHTS];

in vec3		normal;
in vec3		fragPos;
in vec2		textureCoord;

out vec4	final;

vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 camDir);

void main()
{
	vec3 finalColor = vec3(0.0);
	vec3 ambient	= vec3(0.0);

	if(nLights <= MAX_LIGHTS)
	{
		vec3 nNormal	= normalize(normal);
		vec3 nCameraDir = normalize(cameraPos - fragPos);

		float baseIntensity = 2;
		ambient  = 0.2 * vec3(texture(material.diffuse_1, textureCoord));

		for(int i = 0; i < nLights; i++)
		{
			finalColor += calculateLight(lights[i], nNormal, fragPos, nCameraDir) * baseIntensity;
		}
	}
	final = vec4(finalColor, 1.0);
}



vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 camDir)
{
	vec3 lightDirection = normalize(light.position - fragPos);

	// LightDirection depends on the type of light choosen
	if(light.type == LIGHT_DIRECTIONAL)
	{
		lightDirection = normalize(light.direction);
	}

	// Diffuse
	float diff			= max(dot(normal, lightDirection), 0.0);
	vec3 diffuse		= light.diffuse * vec3(texture(material.diffuse_1, textureCoord)) * diff;

	// Specular
	vec3 reflectDir		= reflect(-lightDirection, normal);
	float spec			= pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
	vec3 specular		= light.specular * spec * vec3(texture(material.specular_1, textureCoord));

	// Light specifities
	if(light.type == LIGHT_POINT)
	{
		float distance	= length(light.position - fragPos);
		float attenuation = 1.0 / ( 1.0 + light.linear * distance + light.quadratic * distance * distance );
		return (diffuse + specular) * attenuation;
	}
	else if(light.type == LIGHT_SPOT)
	{
		// Cutoff
		float theta			= dot(lightDirection, normalize(-light.direction));
		float epsilon		= light.cutoff - light.outerCutoff;
		float intensity		= clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

		return (diffuse + specular * 0) * intensity;
	}
	else if(light.type == LIGHT_DIRECTIONAL)
	{
		return (diffuse + specular * 0);
	}
	else 
	{
		return vec3(0.0);
	}
}