#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColor;

uniform vec2 uOffset;
uniform vec2 uScale;
uniform mat4 projection;

out vec4 chCol;

void main()
{
    vec2 scaledPos = inPos * uScale;
    vec2 finalPos = scaledPos + uOffset;
    gl_Position = projection * vec4(finalPos, 0.0, 1.0);        
    chCol = vec4(inColor, 1.0);
}