#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 TexCoord;

uniform mat4 projection;
uniform vec2 uOffset;

void main() {
    TexCoord = aUV;
    gl_Position = projection * vec4(aPos + uOffset, 0.0, 1.0);
}
