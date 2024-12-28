#version 330 core
in vec4 FragPos;
in float vFragDepth;

uniform vec3 lightPos;
uniform float far_plane;
uniform float shadowCamFC;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    lightDistance = lightDistance / far_plane;

    float distance_const = log2(vFragDepth) * shadowCamFC * 0.5;
    gl_FragDepth = lightDistance;

}