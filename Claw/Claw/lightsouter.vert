#version 330 core

layout(location = 0) in vec2 inPos;      // Position
layout(location = 1) in vec2 inUV;       // Texture coordinates

out vec2 fragUV;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(inPos, 0.0, 1.0);
    fragUV = inUV;
}
