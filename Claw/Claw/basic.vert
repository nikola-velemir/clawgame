#version 330 core

layout(location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
  
out vec4 chCol;


uniform mat4 projection;
uniform vec2 uOffset;

void main()
{
    gl_Position = projection * vec4(aPos + uOffset, 0.0, 1.0);
    
    chCol = vec4(aColor,1.0);
}
