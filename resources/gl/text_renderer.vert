#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 texturePosition;

uniform mat4 transformation;

smooth out vec2 textureCoordnate;

void main() {
  gl_Position = transformation * vec4(vertexPosition, 0.0, 1.0);
  textureCoordnate = texturePosition;
}
