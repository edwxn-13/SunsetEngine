#version 450 core

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec3 vCol;
layout(location = 2) in vec3 vNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 col;
out vec3 nor;
out vec3 FragPosWorldSpace;

void main()
{
	gl_Position = projection * view * model * vPos;
	col = vec3(0.0f,0.0f,1.0f);
	nor = mat3(transpose(inverse(model))) * vNor;
	FragPosWorldSpace = vec3(model * vPos);
}
