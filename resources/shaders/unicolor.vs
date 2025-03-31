#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

struct Transformation {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform Transformation t;

out vec3 normal;
out vec3 fragPos;

void main() {
  gl_Position = t.projection * t.view * t.model * vec4(aPos, 1.0);
  normal = normalize( mat3( transpose( inverse( t.model))) * aNormals);

  fragPos = vec3( t.model * vec4(aPos, 1.0) );
}