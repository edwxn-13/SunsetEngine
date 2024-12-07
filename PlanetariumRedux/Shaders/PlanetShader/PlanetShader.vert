#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aTex;
layout(location = 2) in vec3 vNor;
layout(location = 3) in vec3 vCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 camMat;
uniform mat4 projectedLightSpaceMatrix;

out vec3 nor;
out vec2 tex;
out vec3 FragPosWorldSpace;
out vec3 tcol;
out vec4 fragPosLightSpace;
out float vFragDepth;

void main()
{
	tcol = vCol;
	gl_Position = camMat * model * aPos;

	tex = aTex.xy;
	nor =  mat3(transpose(inverse(model))) * vNor;
	FragPosWorldSpace = vec3(model * aPos);

	//light calc
	fragPosLightSpace = projectedLightSpaceMatrix * vec4(FragPosWorldSpace, 1.0);
	vFragDepth = 1.0 + gl_Position.w;
}
