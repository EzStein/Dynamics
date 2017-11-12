#version 450 core
layout (location = 0) in vec2 pos;
uniform mat4 transformation;
void main() {
  gl_Position = transformation * vec4(pos, 0.0, 1.0);
}
