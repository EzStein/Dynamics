#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec4 baseColor;
out vec4 inColor;
void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  inColor = baseColor + vec4(aPos.xyz, 0.0f);
}
