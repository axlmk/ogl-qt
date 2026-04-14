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

    vec4 viewPos = t.view * t.model * vec4(aPos, 1.0);

    // Compute scale factor to counteract perspective scaling
    float distance = abs(viewPos.z); // depth in view space (positive)
    float scale = distance * 3 / 600;

    // Apply uniform scale in view space
    viewPos.xyz *= scale;

  gl_Position = t.projection * t.view * t.model * vec4(aPos, 1.0);

  fragPos = vec3( t.model * vec4(aPos, 1.0) );
}