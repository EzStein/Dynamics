#version 430 core

layout (location = 0) in vec3 pos;

uniform mat4 transformation;

void main() {
  gl_Position = transformation * vec4(pos, 1.0);
}
