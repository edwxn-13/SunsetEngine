#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 camMat;
uniform vec3 sphere_center;
uniform vec3 cam_pos; 

out float vFragDepth;

void main()
{
    gl_Position =  camMat * model * vec4(aPos, 1.0);
	vFragDepth = 1.0 + gl_Position.w;
}