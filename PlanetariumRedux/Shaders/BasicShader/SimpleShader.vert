#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aTex;
layout(location = 2) in vec3 vNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 camMat;

out vec3 nor;
out vec2 tex;
out vec3 FragPos;
out float vFragDepth;

void main()
{
	gl_Position = camMat * model * aPos;
	tex = aTex.xy;
	
	nor =  mat3(transpose(inverse(model))) * vNor;
	FragPos = vec3(model * aPos);
	
	vFragDepth = 1.0 + gl_Position.w;

}
