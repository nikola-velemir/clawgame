#version 330 core

in vec2 TexCoord;
uniform sampler2D uTexture;

out vec4 FragColor;


void main() {
    vec4 texColor =  texture(uTexture, TexCoord);
    FragColor = texColor;
}
