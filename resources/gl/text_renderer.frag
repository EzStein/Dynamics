#version 330 core

smooth in vec2 textureCoordnate;

uniform vec4 textColor;
uniform sampler2D sampler;

out vec4 outColor;

void main() {
  float opacity = texture(sampler, textureCoordnate).r;
  outColor = textColor * vec4(1.0f, 1.0f, 1.0f, opacity);
}
