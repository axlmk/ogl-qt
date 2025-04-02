#version 330 core

layout (location = 0) in vec3 aPos;

struct Transformation {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform Transformation t;

out vec3 fragPos;

void main() {
  gl_Position = t.projection * t.view * t.model * vec4(aPos, 1.0);

  fragPos = vec3( t.model * vec4(aPos, 1.0) );
}