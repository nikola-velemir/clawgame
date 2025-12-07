#version 330 core

layout(location = 0) in vec2 inPos;
layout (location = 1) in vec3 aColor;

uniform vec2 uOffset;
uniform vec2 uScale;
uniform float uRotation; 
uniform vec2 uPivot;     
uniform mat4 projection;
out vec4 chCol;

void main()
{
    vec2 pos = inPos - uPivot;
    
    float s = sin(uRotation);
    float c = cos(uRotation);
    vec2 rotated = vec2(
        pos.x * c - pos.y * s,
        pos.x * s + pos.y * c
    );
    
    rotated += uPivot;
    
    rotated *= uScale;
    
    rotated += uOffset;
    
    gl_Position = projection * vec4(rotated, 0.0, 1.0);
    chCol = vec4(aColor, 1.0);

}