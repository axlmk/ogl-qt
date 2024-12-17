#version 330 core
out vec4 finalColor;

in vec2 textCoord;
in vec4 cpuColor;

uniform sampler2D ourTexture1;

void main() {
  finalColor = texture(ourTexture1, textCoord);
}