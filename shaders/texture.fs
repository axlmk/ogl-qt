#version 330 core
out vec4 finalColor;

in vec2 textCoord;

uniform sampler2D ourTexture;

void main() {
  finalColor = texture(ourTexture, textCoord);
}