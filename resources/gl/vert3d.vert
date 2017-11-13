#version 450 core
layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;
out vec4 col;
void main() {
  gl_Position =  projection * view * model * vec4(pos, 1.0);
  col = color;
}
