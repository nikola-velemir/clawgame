#version 330 core

layout(location = 0) in vec2 inPos;
layout (location = 1) in vec3 aColor;

uniform vec2 uOffset;    // Claw position
uniform vec2 uScale;
uniform float uRotation; // Rotation angle in radians
uniform vec2 uPivot;     // Pivot point in local coordinates
uniform mat4 projection;
out vec4 chCol;

void main()
{
    // Step 1: Translate so pivot is at origin
    vec2 pos = inPos - uPivot;
    
    // Step 2: Apply rotation around origin
    float s = sin(uRotation);
    float c = cos(uRotation);
    vec2 rotated = vec2(
        pos.x * c - pos.y * s,
        pos.x * s + pos.y * c
    );
    
    // Step 3: Translate back
    rotated += uPivot;
    
    // Step 4: Apply scale
    rotated *= uScale;
    
    // Step 5: Apply offset (position at claw)
    rotated += uOffset;
    
    gl_Position = projection * vec4(rotated, 0.0, 1.0);
    chCol = vec4(aColor, 1.0);

}

//#version 330 core
//layout(location = 0) in vec2 inPos;
//
//uniform vec2 uOffset;
//uniform vec2 uScale;
//uniform mat4 projection;
//uniform float uRotation;
//uniform vec2 uPivot;
//
//out vec4 chCol;
//
//void main()
//{
//    vec2 translated = inPos - uPivot;
//
//    float cosTheta = cos(uRotation);
//    float sinTheta = sin(uRotation);
//
//    vec2 rotated = vec2(
//        translated.x * cosTheta - translated.y * sinTheta,
//        translated.x * sinTheta + translated.y * cosTheta
//    );
//
//      vec2 afterRotation = rotated + uPivot;
//    
//    // Apply scale and offset
//    vec2 scaled = afterRotation * uScale;
//    vec2 finalPos = scaled + uOffset;
//    
//    gl_Position = projection * vec4(finalPos, 0.0, 1.0);      
//    chCol = vec4(0.3, 0.3, 0.3, 1.0);
//}