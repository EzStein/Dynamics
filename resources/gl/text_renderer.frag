#version 430 core

smooth in textureCoordnate;

uniform vec4 textColor;
uniform sampler2D sampler;

out outColor;

void main() {
  float opacity = texture(sampler, textureCoordnate).r;
  outColor = textColor * vec4(1.0f, 1.0f, 1.0f, opacity);
}
