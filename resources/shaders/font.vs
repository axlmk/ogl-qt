#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 textCoord;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(vertex.xy, 0.0, 1.0);
	textCoord = vertex.zw;
}