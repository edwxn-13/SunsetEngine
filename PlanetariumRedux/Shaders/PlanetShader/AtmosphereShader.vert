#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vNor;

uniform mat4 model;
uniform mat4 camMat;
uniform vec3 sphere_center;

out VS_OUT {
    float vFragDepth;
    vec3 FragPos;
    vec3 nor;
} vs_out;

void main()
{
    gl_Position = camMat * model * vec4(aPos, 1.0);
    vs_out.nor =  mat3(transpose(inverse(model))) * vNor;
	vs_out.vFragDepth = 1.0 + gl_Position.w;
    vs_out.FragPos = vec3(aPos);
}