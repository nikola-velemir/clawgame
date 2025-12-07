#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in float inIsBottom;
layout(location = 2) in vec3 inColor;
uniform float uXOffset;   // horizontal movement for ALL vertices
uniform float uYOffset;   // vertical movement ONLY for bottom vertices
uniform mat4 projection;

out vec4 chCol;

void main()
{
    float yMove = uYOffset * inIsBottom;       // only bottom moves in Y
    float xMove = uXOffset;                    // all vertices move in X

    vec2 finalPos = vec2(inPos.x + xMove, inPos.y + yMove);

    gl_Position = projection * vec4(finalPos, 0.0, 1.0);
    chCol = vec4(inColor, 1.0);
}