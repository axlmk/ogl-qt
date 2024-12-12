#version 330 core

out vec4 finalColor;
uniform vec4 cpuColor;

void main() {
  finalColor = cpuColor;
}