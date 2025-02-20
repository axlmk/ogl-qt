#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

struct Transformation {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform Transformation t;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = t.projection * t.view * t.model * vec4(aPosition, 1.0);
	textureCoord = aTextCoord;
	normal = mat3( transpose( inverse( t.model))) * aNormal;
	fragPos = vec3( t.model * vec4( aPosition, 1.0));
}