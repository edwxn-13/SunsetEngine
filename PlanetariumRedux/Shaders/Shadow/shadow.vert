#version 330 core

layout (location = 0) in vec4 vPos;

uniform mat4 projectedLightSpaceMatrix;
uniform mat4 model;


void main()
{
    gl_Position = projectedLightSpaceMatrix * model * vPos;    
}

