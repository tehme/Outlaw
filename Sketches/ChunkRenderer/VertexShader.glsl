#version 330 core

layout (location = 0) in vec3 inPosition;

void main()
{
    gl_Position = vec4(inPosition.xyz, 1.0);
}
